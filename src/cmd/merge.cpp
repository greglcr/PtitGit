#include "../lib/object.h"
#include "../lib/object_commit.h"
#include "../lib/object_tree.h"
#include "../lib/object_file.h"
#include "../lib/repos.h"
#include "../lib/reftag.h"
#include "merge.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

void merge(std::string commitName, std::string message){
    PtitGitRepos X = PtitGitRepos();
    Commit CNew = Commit().fromfile(objectFind(X, commitName, true, "commit"));
    Commit COld = Commit().fromfile(ref_resolve(X,X.getWorkingFolder() / ".ptitgit" / "HEAD"));
    Tree TreeNew = CNew.getTree(), TreeOld = COld.getTree();
    Tree T = mergeTree(TreeOld, TreeNew);
    Commit C = Commit(T,{CNew.getHashedContent(),COld.getHashedContent()},"","",message);

    C.writeObject();
    std::string content = X.get_repos_content("HEAD");
    if(content[0] == 'r' && content[1] == 'e' && content[2] == 'f' && content[3] == ':' && content[4] == ' '){
        fs::path path = content.substr(5);
        std::ofstream out(path);
        out << C.getHashedContent();
        std::cout<<"The hash of the commit for branch "<<path.filename()<<" is now "<<C.getHashedContent()<<std::endl;
        INDEXreset(C);
        return;        
    }
    else{
        std::ofstream out(X.getWorkingFolder() / ".ptitgit" / "HEAD");
        out << C.getHashedContent();
        std::cout<<"The hash of the commit for the detached HEAD is now "<<C.getHashedContent()<<std::endl;
        INDEXreset(C);
        return;
    }
}

Tree mergeTree(Tree T1, Tree T2){
    std::vector <Tree> V, V1 = T1.get_trees_inside(), V2 = T2.get_trees_inside();
    std::vector <File> F, F1 = T1.get_blobs_inside(), F2 = T2.get_blobs_inside();
    V.clear();F.clear();

    long long count1 = 0, count2 = 0;
    while(count1 + 1 < (long long) V1.size() || count2 + 1 < (long long) V2.size()){
        if(count2 + 1 == (long long) V2.size() || V1[count1].get_folder_path().filename().string() < V2[count2].get_folder_path().filename().string()){
            V.push_back(V1[count1]);count1++;
        }
        else if(count1 + 1 == (long long) V1.size() || V2[count2].get_folder_path().filename().string() < V1[count1].get_folder_path().filename().string()){
            V.push_back(V2[count2]);count2++;
        }
        else{
            V.push_back(mergeTree(V1[count1],V2[count2]));
        }
    }
    sort(V.begin(),V.end(),compareTree);

    count1 = 0; count2 = 0;
    while(count1 + 1 < (long long) F1.size() || count2 + 1 < (long long) F2.size()){
        if(count2 + 1 == (long long) F2.size() || F1[count1].get_file_path().filename().string() < F2[count2].get_file_path().filename().string()){
            F.push_back(F1[count1]);count1++;
        }
        else if(count1 + 1 == (long long)  F1.size() || F2[count2].get_file_path().filename().string() < F1[count1].get_file_path().filename().string()){
            F.push_back(F2[count2]);count2++;
        }
        else{
            std::string S, S1 = F1[count1].getActualFile(), S2 = F2[count2].getActualFile();
            S = "There is a conflict in this file. Please merge manually!\n\nHere is the old file:\n" + S1 + "\n\nHere is the new file:\n" + S2;
            fs::path path = PtitGitRepos().getWorkingFolder() / F1[count1].get_file_path();
            std::ofstream out(path);
            out << S;
            F.push_back(File(path));
            File(path).writeObject();
            count1++;count2++;
        }
    }
    sort(F.begin(),F.end(),compareFile);

    std::string content = T1.get_folder_path(); content += '\n';
    for(long long i = 0; i < (long long) V.size(); i++) content += "tree " + V[i].getHashedContent() + " " + V[i].get_folder_path().filename().string() + "\n";
    for(long long i = 0; i < (long long) F.size(); i++) content += "file " + F[i].getHashedContent() + " " + F[i].get_file_path().filename().string() + "\n";

    std::string abc = content;
    content = "tree " + std::to_string(abc.size()) + '\n' + abc;

    Tree TT = Tree().createTreeFromContent(content,false);
    TT.writeObject();
    return TT;
}