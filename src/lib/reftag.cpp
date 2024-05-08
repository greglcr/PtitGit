#include "reftag.h"
#include "repos.h"
#include "object.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

std::string ref_resolve(PtitGitRepos X, fs::path path){
    fs::path repo_path = X.getWorkingFolder();
    fs::path fun = repo_path / ".ptitgit" / path;

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