#include "reftag.h"
#include "repos.h"
#include "object.h"
#include "object_commit.h"
#include "hashing.h"

#include <map>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string ref_resolve(PtitGitRepos X, fs::path path){
    fs::path fun;
    fs::path repo_path = X.getWorkingFolder();
    if(path.is_absolute()){
        fs::path path2 = path;
        fs::path parent = path2.parent_path();
        while(parent!=path2){
            if(path2 == repo_path / ".ptitgit") break;
            path2 = parent;
            parent = path2.parent_path();
        }
        if(path2 != repo_path / ".ptitgit"){std::cerr<<"Error: Bad path";exit(0);}
        fun = path;
    }
    else if(std::string(path).substr(0,8) == ".ptitgit") fun = repo_path / path;
    else fun = repo_path / ".ptitgit" / path;

    if(!fs::exists(fun)){std::cerr<<"Error: Nothing to resolve"<<std::endl<<fun<<std::endl;exit(0);}
    std::ifstream fileToShow(fun);

    if(!fileToShow.is_open()) {
        std::cerr << "Error: Broken ref" << std::endl;
        exit(0);
    }

    std::stringstream buffer;
    buffer << fileToShow.rdbuf();
    std::string content = buffer.str();
    fileToShow.close();

    if (content.back() == '\n')
        content.pop_back();
    
    if(content[0]=='r' && content[1]=='e' && content[2]=='f' && content[3]==':' && content[4]==' '){
        content.erase(0,5);
        return ref_resolve(X,content);
    }
    else return content;
}

std::map <fs::path, std::string> ref_list_basic(PtitGitRepos X){
    fs::path fun = X.getWorkingFolder() / ".ptitgit" / "refs";
    return ref_list({},X,fun);
}

std::map <fs::path, std::string> ref_list(std::map <fs::path, std::string> current, PtitGitRepos X, fs::path path){
    for (const auto& entry : fs::directory_iterator(path)) {
        if(fs::is_regular_file(entry.path())){
            current.insert({entry.path(), ref_resolve(X,entry.path())});
        }
        else if(fs::is_directory(entry.path())){
            current.merge(ref_list({},X,entry.path()));
        }
    }
    return current;
}

Tag::Tag(Object tagObject, std::string tagType, std::string tagAuthor, std::string tagger, std::string tagName, std::string tagMessage){
    this->tagObject = tagObject;
    this->tagAuthor = tagAuthor;
    this->tagger = tagger;
    this->tagName = tagName;
    this->tagMessage = tagMessage;
    this->tagType = tagType;

    std::string abcabc =  "object_type " + this->tagType + "\nauthor " + this->tagAuthor + "\ntagger " + this->tagger + "\ntag_object " + this->tagObject.getHashedContent() + "\ntag_name " + this->tagName + "\n" + this->tagMessage;
    long long uu = abcabc.size();
    this->content = "tag " + std::to_string(uu) + "\n" + abcabc;
    this->hashedContent = hashString(this->content);
}

Object Tag::getObject(){
    return this->tagObject;
}

void Tag::calculateContent(){
    std::string abcabc =  "object_type " + this->tagType + "\nauthor " + this->tagAuthor + "\ntagger " + this->tagger + "\ntag_object " + this->tagObject.getHashedContent() + "\ntag_name " + this->tagName + "\n" + this->tagMessage;
    long long uu = abcabc.size();
    this->content = "tag " + std::to_string(uu) + "\n" + abcabc;
    this->hashedContent = hashString(this->content);
}

void Tag::tag_create(PtitGitRepos X, std::string tag_name, std::string tagged_object, std::string tag_message, bool create){
    //tagged_object = objectFind(X,tagged_object,true,"object",true);
    if(create){
        std::string str = X.get_object_content(tagged_object);
        long long abcd = str.find(' ');
        long long dcba = str.find('\n');
        if( (long long) std::stoi(str.substr(abcd+1,dcba-abcd-1)) != (long long) str.size()-dcba-1) std::cerr<<"Bad size!";
        if(str.substr(0,abcd) == "commit"){
            this->tagObject = Commit().fromstring(str.substr(dcba+1));
            this->tagName = tag_name;
            this->tagType = "commit";
            this->tagMessage = tag_message;
            this->calculateContent();
            this->writeObject();
            writeRef(this->tagName, this->hashedContent);
        }
        else if(str.substr(0,abcd) == "tag"){
            this->tagObject = Tag().fromstring(str.substr(dcba+1));
            this->tagName = tag_name;
            this->tagType = "tag";
            this->tagMessage = tag_message;
            this->calculateContent();
            this->writeObject();
            writeRef(this->tagName, this->hashedContent);
        }
        else if(str.substr(0,abcd) == "tree"){
            //this->tagObject = Tree(X.getWorkingFolder() / ".ptitgit" / "objects" / get_path_to_object(tagged_object));
            long long abba = str.find('\n', dcba+1);
            this->tagObject = Tree(str.substr(dcba+1, abba-dcba-1));
            this->tagName = tag_name;
            this->tagType = "tree";
            this->tagMessage = tag_message;
            this->calculateContent();
            this->writeObject();
            writeRef(this->tagName, this->hashedContent);
        }
        else if(str.substr(0,abcd) == "file"){
            //this->tagObject = Tree(X.getWorkingFolder() / ".ptitgit" / "objects" / get_path_to_object(tagged_object));
            long long abba = str.find('\n', dcba+1);
            this->tagObject = File(str.substr(dcba+1, abba-dcba-1));
            this->tagName = tag_name;
            this->tagType = "file";
            this->tagMessage = tag_message;
            this->calculateContent();
            this->writeObject();
            writeRef(this->tagName, this->hashedContent);
        }
        else{std::cerr<<"Tagging not supported!";return;}

    }
    else writeRef(tag_name, tagged_object);
}

Tag Tag::fromfile(std::string hashedContent){
    const std::ifstream input_stream(PtitGitRepos().getWorkingFolder() / ".ptitgit" / "objects" / get_path_to_object(hashedContent), std::ios_base::binary);

    if (input_stream.fail()) {
        throw std::runtime_error("Failed to open file");
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();

    long long abcd = buffer.str().find(' ');
    long long dcba = buffer.str().find('\n');

    if(buffer.str().substr(0,abcd) != "tag") std::cerr<<"Not a tag!";
    if((long long) stoi(buffer.str().substr(abcd+1,dcba-abcd-1)) != (long long) buffer.str().size()-dcba-1) std::cerr<<"Bad size!";

    return Tag::fromstring(buffer.str().substr(dcba+1));
}

Tag Tag::fromstring(std::string commitContent){
    Tag X = Tag();
    long long abc = commitContent.find(' ');
    long long cba = commitContent.find('\n');
    X.tagType = commitContent.substr(abc+1,cba-abc-1);
    long long mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    X.tagAuthor = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    X.tagger = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    std::string abcxyz = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;abc = commitContent.find(' ',mm);cba = commitContent.find('\n',mm);
    X.tagName = commitContent.substr(abc+1,cba-abc-1);
    mm = cba+1;cba = commitContent.find('\n',mm);
    X.tagMessage = commitContent.substr(mm,cba-mm);
    if(X.tagType == "commit") X.tagObject = Commit().fromfile(abcxyz);
    else if(X.tagType == "tag") X.tagObject = Tag().fromfile(abcxyz);
    else if(X.tagType == "tree") X.tagObject = findTree(abcxyz);
    else if(X.tagType == "file") X.tagObject = findFile(abcxyz);
    else std::cerr<<"Error!";
    X.calculateContent();
    return X;
}

void writeRef(std::string refName, std::string refString){
    fs::path path = PtitGitRepos().getWorkingFolder() / ".ptitgit" / "refs" / "tags" / refName;
    std::ofstream out(path);
    out << refString;
    return;
}

void writeBranch(std::string refName, std::string refString){
    fs::path path = PtitGitRepos().getWorkingFolder() / ".ptitgit" / "refs" / "heads" / refName;
    std::ofstream out(path);
    out << refString;
    return;
}

std::vector <std::string> objectResolve(PtitGitRepos X,std::string name, bool short_hash){
    std::vector <std::string> V;V.clear();
    if(name == "HEAD"){
        V.push_back(ref_resolve(X, X.getWorkingFolder() / ".ptitgit" / "HEAD"));
        return V;
    }
    std::string prefix = name.substr(0,2);
    std::string rest = name.substr(2);
    fs::path path = X.getWorkingFolder() / ".ptitgit" / "objects" / prefix;
    std::string need_check = std::string(X.getWorkingFolder() / ".ptitgit" / "objects" / prefix / rest);
    if(short_hash == true && fs::exists(path)) for (const auto & entry : fs::directory_iterator(path)){
        std::string contender = std::string(entry.path());
        auto res = std::mismatch(need_check.begin(),need_check.end(),contender.begin());
        if(res.first == need_check.end()){
            long long abc = contender.find(".ptitgit/objects/");
            V.push_back(contender.substr(abc+17,2) + contender.substr(abc+20));
        }
    }
    if(fs::exists(X.getWorkingFolder() / ".ptitgit" / "refs" / "heads" / name)) V.push_back(ref_resolve(X,X.getWorkingFolder() / ".ptitgit" / "refs" / "heads" / name));
    if(fs::exists(X.getWorkingFolder() / ".ptitgit" / "refs" / "tags" / name)) V.push_back(ref_resolve(X,X.getWorkingFolder() / ".ptitgit" / "refs" / "tags" / name));
    return V;
}

std::string objectFind(PtitGitRepos X,std::string name,bool short_hash, std::string type,bool follow){
    std::vector <std::string> V = objectResolve(X,name,short_hash),W;W.clear();
    if(name == "HEAD") return V[0];
    long long jj;
    for(jj=0;jj< (long long) V.size();jj++){
        std::string obj = V[jj];
        while(true){
            std::string content = X.get_object_content(obj);
            long long abc = content.find(' ');
            if(content.substr(0,abc) == "tag" && follow == true){
                Tag G = Tag();
                G.fromstring(content);
                obj = G.getObject().getHashedContent();
                continue;
            }
            else if(content.substr(0,abc) == "tag" && type != "tag"){
                Tag G = Tag().fromstring(content);
                obj = G.getObject().getHashedContent();
                continue;
            }
            else if(content.substr(0,abc) == "commit" && type == "tree"){
                Commit C = Commit().fromstring(content);
                W.push_back(C.getTree().getHashedContent());
                break; 
            }
            else{W.push_back(obj);break;}
        }
    }
    if(W.size()>1 && short_hash == false){std::cerr<<"Misleading name!"<<std::endl;exit(0);}
    else if(W.size()>1) return objectFind(X,name,false,type,follow);
    else if(W.size()==0){std::cerr<<"Nothing match"<<std::endl;exit(0);}
    else return W[0];
}
