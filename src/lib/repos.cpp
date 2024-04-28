#include "repos.h"

#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

PtitGitRepos::PtitGitRepos() {

    fs::path curPath = fs::current_path();

    while (curPath != curPath.root_directory() && !fs::exists(curPath / ".ptitgit")) {
        curPath = curPath.parent_path();
    }

    if (!fs::exists(curPath / ".ptitgit")) {
        std::cerr << "Erreur : Pas de repos git dans les dossiers parents" << std::endl;
        exit(0);
    }
    else {
        this->workingFolder = curPath;
    }
    
}

PtitGitRepos::PtitGitRepos(fs::path workingFolder) {

    if(workingFolder.is_relative()) {
        workingFolder = fs::current_path() / workingFolder;
    }

    if(!fs::exists(workingFolder / ".ptitgit")) {
        std::cerr << "Erreur : Le dossier indiqué n'est pas un repos git" << std::endl;
        exit(0);
    }
    else {
        this->workingFolder = workingFolder;
    }

}

void PtitGitRepos::writeObject(Object objectToWrite) {

    fs::path pathToWrite = this->workingFolder / ".ptitgit" / "objects" / objectToWrite.getPathToWrite();

    if (!fs::exists(pathToWrite.parent_path())) {
        fs::create_directory(pathToWrite.parent_path());
    }

    std::ofstream fileToWrite(pathToWrite);
    
    if (!fileToWrite) {
        std::cerr << "Error : Impossible d'ouvrir le fichier demandé dans writeObject";
        exit(0);
    }

    fileToWrite << objectToWrite.getContent();
    fileToWrite.close();

}