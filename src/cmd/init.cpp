#include <iostream>
#include <filesystem>
#include <vector>
#include "init.h"

/* In order to handle file manipulation, we use the filesystem library introduced in C++17*/

PtiteGitInit::PtiteGitInit () {
    if (std::filesystem::exists(".ptitgit")) {
        std::cerr << "The current directory is already a git repos" << std::endl;
    }
    else {
        try {
            std::filesystem::create_directory(".ptitgit");
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

PtiteGitInit::PtiteGitInit (std::filesystem::path folderToInit) {
    if(std::filesystem::exists(folderToInit / ".ptitgit")) {
        std::cerr << "The given directory is already a git repos" << std::endl;
    }
    else {
        try {
            std::filesystem::create_directory(folderToInit / ".ptitgit");
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void init() {
    PtiteGitInit myGitInit;
}

void init(std::filesystem::path folderToInit) {
    PtiteGitInit myGitInit(folderToInit);
}
