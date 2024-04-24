#include "object.h"
#include "object_file.h"
#include "object_tree.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

Tree::Tree(fs::path pathFolder) {

    if (!fs::exists(pathFolder)) {
        std::cerr << "Error in Tree construction : The given path doesn't exists (" << pathFolder << ")" << std::endl;
    }
    if (!fs::is_directory(pathFolder)) {
        std::cerr << "Error in Tree construction : The given path is not a directory (" << pathFolder << ")" << std::endl;
    }
    
}