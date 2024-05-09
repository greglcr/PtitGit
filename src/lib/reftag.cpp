#include "reftag.h"
#include "repos.h"
#include "object.h"

#include <map>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

std::string ref_resolve(PtitGitRepos X, fs::path path){
    fs::path fun;
    fs::path repo_path = X.getWorkingFolder();
    if(path.is_absolute()){
        fs::path path2 = path;
        fs::path parent = path2.parent_path();
        while(parent!=path2){
            if(path2 == repo_path) break;
            path2 = parent;
            parent = path2.parent_path();
        }
        if(path2 != repo_path){std::cerr<<"Error: Bad path";exit(0);}
        fun = path;
    }
    else fun = repo_path / ".ptitgit" / path;

    if(!fs::exists(fun)){std::cerr<<"Error: Nothing to resolve";exit(0);}
    std::ifstream fileToShow(fun);

    if(!fileToShow.is_open()) {
        std::cerr << "Error: Broken ref" << std::endl;
        exit(0);
    }

    std::stringstream buffer;
    buffer << fileToShow.rdbuf();
    std::string content = buffer.str();
    fileToShow.close();
    
    if(content[0]=='r' && content[1]=='e' && content[2]=='f' && content[3]==':' && content[4]==' '){
        content.erase(0,5);
        return ref_resolve(X,content);
    }
    else return content;
}

std::map <fs::path, std::string> ref_list(PtitGitRepos X){
    fs::path fun = X.getWorkingFolder() / ".ptitgit" / "refs";
    return ref_list(X,fun);
}

std::map <fs::path, std::string> ref_list(PtitGitRepos X, fs::path path=".", std::map <fs::path, std::string> current){
    for (const auto& entry : fs::directory_iterator(path)) {
        if(fs::is_regular_file(entry.path())){
            current.insert({entry.path(), ref_resolve(X,entry.path())});
        }
        else if(fs::is_directory(entry.path())){
            current.merge(ref_list(X,entry.path()));
        }
    }
    return current;
}