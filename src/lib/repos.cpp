#include "repos.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

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

std::string PtitGitRepos::get_object_content(std::string hashedContent) {

    fs::path pathToObject = this->workingFolder / ".ptitgit" / "objects" / get_path_to_object(hashedContent);
    std::ifstream fileToShow(pathToObject);

    if(!fileToShow.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier voulu dans get_object_content (" << pathToObject << ")" << std::endl;
        exit(0);
    }

    std::stringstream buffer;
    buffer << fileToShow.rdbuf();
    std::string content = buffer.str();
    fileToShow.close();

    return content;

}