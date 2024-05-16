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

fs::path PtitGitRepos::getWorkingFolder(){
    return this->workingFolder;
}



std::string PtitGitRepos::get_config(std::string key) {
    fs::path working_folder = this->getWorkingFolder();
    std::ifstream config_file;
    config_file.open(working_folder/".ptitgit/config");
    

    if ( config_file.is_open() ) {
        while ( config_file ) {
            std::string line;
            std::getline (config_file, line);
            if (line.length() > 0 and line[0] == '#') continue;

            int split = line.find('=');
            if (split == -1) continue;

            std::string key_i = line.substr(0, line.find('='));
            if (key_i == key)
                return line.substr(split+1, std::string::npos);
        }
    }
    return "";
}

std::string PtitGitRepos::get_repos_content(fs::path filePath) {

    filePath = this->workingFolder / ".ptitgit" / filePath;

    if (!fs::exists(filePath)) {
        std::cerr << "Error : invalid file path in get_repos_content (" << filePath << ")" << std::endl;
        std::ifstream fileStream(filePath);
        exit(0);
    }

    std::ifstream fileStream(filePath);
    
    if(!fileStream.is_open()) {
        std::cerr << "Error : impossible to open the given file in get_repos_content" << std::endl;
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    std::string content = buffer.str();
    fileStream.close();    

    return content;
}


void PtitGitRepos::set_config(std::string key, std::string value) {
    fs::path working_folder = this->getWorkingFolder();
    std::ifstream config_file;
    config_file.open(working_folder/".ptitgit/config");
    
    bool added = false;
    std::string result = "";

    if ( config_file.is_open() ) {
        while ( config_file ) {
            std::string line;
            std::getline (config_file, line);

            if (line.length() > 0 and line[0] == '#')   {
                result += line + "\n";
                continue;
            }

            int split = line.find('=');
            if (split == -1) {
                result += line + "\n";
                continue;
            }

            std::string key_i = line.substr(0, line.find('='));
            if (key_i != key)   {
                result += line + "\n";
            } else  {
                result += key+"="+value+"\n";
                added = true;
            }
        }

        config_file.close();

        if (!added) {
            result.pop_back();
            result += key+"="+value+"\n";
        }
        
        if (result.length() >= 2 and result.substr(result.size()-2) == "\n\n")
            result.pop_back();
        
        std::ofstream config_out;
        config_out.open(working_folder/".ptitgit/config");
        if (config_out.is_open())   {
            config_out << result;
            config_out.close();
        }   else    {
            std::cerr << "NOT ABLE TO WRITE NEW VALUE IN THE CONFIG FILE\n" << result << std::endl;
        }
    }   else
        std::cerr << "unable to open the config file" << std::endl;
}