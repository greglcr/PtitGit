#include "compare_repos.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

bool copy_all_objects(std::string src, std::string target) {
    src += "/.ptitgit/objects";
    target += "/.ptitgit/objects";

    std::cout << src << "  into   " << target << std::endl;

    try {
        fs::copy(src, target, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    } catch (std::exception& e)   {
        std::cout << e.what();
        return false;
    }
    return true;
}
