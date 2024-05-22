#include "hashing.h"
#include "object.h"
#include "object_commit.h"
#include "object_tree.h"
#include "object_file.h"
#include "reftag.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

Commit::Commit(Tree parentTree, std::vector<std::string> parentCommitsHash, std::string commitAuthor, std::string committer, std::string message, std::string gpgsig){

    this->commitAuthor = commitAuthor;
    this->committer = committer;
    this->parentCommitsHash = parentCommitsHash;
    this->parentTree = parentTree;
    this->gpgsig = gpgsig;
    this->message = message;

    std::string abcabc = "tree " + this->parentTree.getHashedContent() + "\n";
    for(long long ii = 0; ii < (long long) this->parentCommitsHash.size(); ii++) abcabc += ("parent " + this->parentCommitsHash[ii] + "\n" );
    abcabc += ("author " + this->commitAuthor + "\ncommitter " + this->committer + "\ngpgsig " + this->gpgsig + "\n" + this->message + "\n");
    
    long long uu = abcabc.size();
    this->content = "commit " + std::to_string(uu) + "\n" + abcabc;
    this->hashedContent = hashString(this->content);
}
Commit::Commit()    {
    this->commitAuthor = "";
    this->committer = "";
    this->message = "";
    this->gpgsig = "";
    this->parentTree = Tree();;
}

void Commit::calculateContent(){
    std::string abcabc = "tree " + this->parentTree.getHashedContent() + "\n";
    for(long long ii = 0; ii < (long long) this->parentCommitsHash.size(); ii++) abcabc += ("parent " + this->parentCommitsHash[ii] + "\n" );
    abcabc += ("author " + this->commitAuthor + "\ncommitter " + this->committer + "\ngpgsig " + this->gpgsig + "\n" + this->message + "\n");
    
    long long uu = abcabc.size();
    this->content = "commit " + std::to_string(uu) + "\n" + abcabc;
    this->hashedContent = hashString(this->content);
}

Commit Commit::fromfile(std::string hashedContent, PtitGitRepos repos){
    const std::ifstream input_stream(repos.getWorkingFolder() / ".ptitgit" / "objects" / get_path_to_object(hashedContent), std::ios_base::binary);
    if (input_stream.fail()) {
        throw std::runtime_error("Failed to open file");
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();

    long long abcd = buffer.str().find(' ');
    long long dcba = buffer.str().find('\n');

    if(buffer.str().substr(0,abcd) != "commit") std::cerr<<"Not a commit!";
    if((long long) stoi(buffer.str().substr(abcd+1,dcba-abcd-1)) != (long long) (buffer.str().size()-dcba-1)) std::cerr<<"Bad size!"<<std::endl<<stoi(buffer.str().substr(abcd+1,dcba-abcd-1))<<" "<<(long long) (buffer.str().size()-dcba-1)<<std::endl;

    return Commit::fromstring(buffer.str().substr(dcba+1), repos);
}

Commit Commit::fromstring(std::string commitContent, PtitGitRepos repos){
    //Commit X = Commit();
    this->parentCommitsHash.clear();
    long long abc = commitContent.find(' ');
    long long cba = commitContent.find('\n');
    long long mm = cba + 1;
    //if(commitContent.substr(mm,abc-mm) != "tree") std::cerr<<"Not a tree :(\n"<<commitContent.substr(mm,abc-mm)<<"\n";

    this->parentTree = findTree(commitContent.substr(abc+1, cba-abc-1),false,repos);
    mm = cba+1;
    abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    while(commitContent.substr(mm,abc-mm)=="parent"){
        std::string xyz = commitContent.substr(abc+1,cba-abc-1);
        this->parentCommitsHash.push_back(xyz);
        mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    }
    this->commitAuthor = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    this->committer = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    this->gpgsig = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;cba = commitContent.find('\n',mm);
    this->message = commitContent.substr(mm,cba-mm);
    this->calculateContent();
    return *this;
}

Tree Commit::getTree(){
    return this->parentTree;
}


std::vector<std::string> Commit::get_parents_hash() {
    return this->parentCommitsHash;
}
/*
void Commit::writeCommit(){
    fs::path curPath = fs::current_path();
    while (curPath != curPath.root_directory() && !fs::exists(curPath / ".ptitgit")) {
        curPath = curPath.parent_path();
    }
    fs::path path = curPath / ".ptitgit" / "objects" / this->getPathToWrite();
    fs::create_directory(curPath / ".ptitgit" / "objects" / std::string(this->getPathToWrite()).substr(0,2));
    std::ofstream out(path);
    out << this->content;
    return;
}*/

void checkout(std::string committ, fs::path placeToWrite, bool force){
    std::string commit = objectFind(PtitGitRepos(), committ, true, "commit");
    Commit C = Commit().fromfile(commit);
    Tree T = C.getTree();
    tree_checkout(T,placeToWrite,force);

    fs::path path = PtitGitRepos().getWorkingFolder() / ".ptitgit" / "refs" / "heads" / committ;
    fs::path pathHEAD = PtitGitRepos().getWorkingFolder() / ".ptitgit" / "HEAD";
    if(fs::exists(path)){
        std::ofstream out(pathHEAD);
        out << "ref: " + std::string(relativeToRepo(path));
        std::cout<<"HEAD is attached to branch "<<committ<<'\n';
    }
    else{
        std::ofstream out(pathHEAD);
        out << C.getHashedContent();
        std::cout<<"Detached HEAD state\n";
    }
    INDEXreset(C);
}

void tree_checkout(Tree T, fs::path placeToWrite, bool force){
    std::string fileContent;
    if(!fs::exists(placeToWrite)) fs::create_directory(placeToWrite);
    if(!fs::is_empty(placeToWrite) && force == false) std::cerr<<"The directory given is not empty\n";
    std::vector<File> V = T.get_blobs_inside();long long kk;
    for(kk = 0; kk < (long long) V.size(); kk++){
        fs::path path = V[kk].get_file_path().filename();
        fileContent = V[kk].getContent();
        long long findEndl = fileContent.find('\n');
        long long findNextEndl = fileContent.find('\n', findEndl + 1);

        std::ofstream out(placeToWrite / path);
        out << V[kk].getContent().substr(findNextEndl + 1);
    }
    std::vector<Tree> VV = T.get_trees_inside();long long kkk;
    for(kkk = 0; kkk < (long long) VV.size(); kkk++){
        tree_checkout(VV[kkk], placeToWrite / VV[kkk].get_folder_path().filename(), force);
    }
    //for(std::vector<Tree>::iterator it = T.get_trees_inside().begin(); it!=T.get_trees_inside().nd(); ++it)
    //    tree_checkout(*it,placeToWrite / it->get_folder_path().parent_path().filename(), force);e
}

std::string Commit::get_hash_parent_tree() {

    return this->parentTree.getHashedContent();

}

void add_files(std::string curHash, PtitGitRepos &curRepos) {

    fs::path folderToInit = curRepos.getWorkingFolder();
    fs::path folderToObject = folderToInit / ".ptitgit/index" / get_folder_to_object(curHash);
    if (!fs::exists(folderToObject)) {
        fs::create_directory(folderToObject);
    }

    fs::copy_file(folderToInit / ".ptitgit/objects" / get_path_to_object(curHash), folderToInit / ".ptitgit/index" / get_path_to_object(curHash));

    //First we must get the file type
    std::string contentFile = curRepos.get_object_content(curHash);
    size_t pos = contentFile.find(' ');
    std::string fileType = contentFile.substr(0, pos);
    if (fileType == "tree") {
        get_next_line(contentFile);
        get_next_line(contentFile);
        while (contentFile != "") {
            std::string curLine = get_next_line(contentFile);
            add_files(get_object_hash(curLine), curRepos);
        }
    }

}

void INDEXreset(Commit C){

    fs::path folderToInit = PtitGitRepos().getWorkingFolder();
    fs::remove_all(folderToInit / ".ptitgit/index");
    fs::create_directory(folderToInit / ".ptitgit/index");

    PtitGitRepos curRepos = PtitGitRepos(folderToInit);
    add_files(C.get_hash_parent_tree(), curRepos);

    //fs::create_directory(folderToInit / ".ptitgit/index" / get_folder_to_object(C.get_hash_parent_tree()));
    //fs::copy_file(folderToInit / ".ptitgit/objects" / get_path_to_object(C.get_hash_parent_tree()), folderToInit / ".ptitgit/index" / get_path_to_object(C.get_hash_parent_tree()));
    
    std::ofstream INDEX(".ptitgit/index/INDEX");
    INDEX << C.get_hash_parent_tree();
    INDEX.close();

}
