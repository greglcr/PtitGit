#include "hashing.h"
#include "object.h"
#include "object_file.h"
#include "object_tree.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

Tree::Tree(fs::path folderPath) {

    if (!fs::exists(folderPath)) {
        std::cerr << "Error in Tree construction : The given path doesn't exists (" << folderPath << ")" << std::endl;
    }
    if (!fs::is_directory(folderPath)) {
        std::cerr << "Error in Tree construction : The given path is not a directory (" << folderPath << ")" << std::endl;
    }
    
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            File curFile = File(folderPath / entry.path().filename());
            this->filesInside.push_back(curFile);
            this->content += "file " + curFile.getHashedContent() + " " + entry.path().filename().string() + "\n";
        }
        else if (entry.is_directory()) {
            Tree curFolder = Tree(folderPath / entry.path().filename().string());
            this->treesInside.push_back(curFolder);
            this->content += "tree " + curFolder.getHashedContent() + " " + entry.path().filename().string() + "\n";
        }
    }

    this->hashedContent = hashString(this->content);

}