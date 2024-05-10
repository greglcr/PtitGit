#include "init.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

/* In order to handle file manipulation, we use the filesystem library introduced in C++17*/

namespace fs = std::filesystem;

void init() {
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

    std::ofstream config(".ptitgit/config");
    config << "# this is the configuration file\n# it contains global associations key=value\n";
    config.close();
}

void init(fs::path folderToInit) {
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


    std::ofstream config(folderToInit / ".ptitgit/config");
    config << "# this is the configuration file\n# it contains global associations key=value\n";
    config.close();
}
