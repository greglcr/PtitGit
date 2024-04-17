#include "repos.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

PtitGitRepos::PtitGitRepos() {

    fs::path curPath = fs::current_path();

    while (curPath != curPath.root_name() && !fs::exists(curPath / ".ptitgit")) {
        curPath.parent_path();
    }

    if (!fs::exists(curPath / ".ptitgit")) {
        std::cerr << "Erreur : Pas de repos git dans les dossiers parents" << std::endl;
    }
    else {
        this->workingFolder = curPath;
    }
    
}

PtitGitRepos::PtitGitRepos(fs::path workingFolder) {

    if(!fs::exists(workingFolder / ".git")) {
        std::cerr << "Erreur : Le dossier indiqué n'est pas un repos git" << std::endl;
    }
    else {
        this->workingFolder = workingFolder;
    }

}