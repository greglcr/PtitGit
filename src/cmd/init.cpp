#include <iostream>
#include <filesystem>
#include <vector>
#include "init.h"

/* In order to handle file manipulation, we use the filesystem library introduced in C++17*/

namespace fs = std::filesystem;

PtiteGitInit::PtiteGitInit () {
    if (fs::exists(".ptitgit")) {
        std::cerr << "The current directory is already a git repos" << std::endl;
    }
    else {
        try {
            for(size_t i = 0; i < FOLDERS_TO_CREATE.size(); i++) {
                fs::create_directory(FOLDERS_TO_CREATE[i]);
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

PtiteGitInit::PtiteGitInit (fs::path folderToInit) {
    if(fs::exists(folderToInit / ".ptitgit")) {
        std::cerr << "The given directory is already a git repos" << std::endl;
    }
    else {
        try {
            for(size_t i = 0; i < FOLDERS_TO_CREATE.size(); i++) {
                fs::create_directories(folderToInit / FOLDERS_TO_CREATE[i]);
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void init() {
    PtiteGitInit myGitInit;
}

void init(fs::path folderToInit) {
    PtiteGitInit myGitInit(folderToInit);
}
