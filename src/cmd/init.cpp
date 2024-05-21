#include "init.h"
#include "../lib/object_commit.h"
#include "../lib/object_tree.h"
#include "../lib/object.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

/* In order to handle file manipulation, we use the filesystem library introduced in C++17*/

namespace fs = std::filesystem;

void init() {

    init(fs::current_path());

}

void init(fs::path folderToInit) {
    if(fs::exists(folderToInit / ".ptitgit")) {
        std::cerr << "The given directory is already a git repos" << std::endl;
        return;
    }
    try {
        for(size_t i = 0; i < FOLDERS_TO_CREATE.size(); i++) {
            fs::create_directories(folderToInit / FOLDERS_TO_CREATE[i]);
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    Tree T = Tree(folderToInit , true);
    Commit C = Commit(T, {}, "PtiteGit team", "PtiteGit team", "First commit");
    C.writeObject();

    fs::path path = folderToInit / ".ptitgit" / "refs" / "heads" / "main";
    std::ofstream out(path);
    out << C.getHashedContent();
    //return;

    fs::path headPath = folderToInit / ".ptitgit" / "HEAD";
    std::ofstream outHead(headPath);
    std::string indirectPath = "ref: " + std::string(relativeToRepo(path));
    outHead << indirectPath;

    std::ofstream config(folderToInit / ".ptitgit/config");
    config << "# this is the configuration file\n# it contains global associations key=value\n";
    config.close();

    INDEXreset(C);
    /*
    The INDEXreset function, defined in object_commit.cpp file, will replace the part below
    std::ofstream INDEX(".ptitgit/index/INDEX");
    INDEX << C.get_hash_parent_tree();
    INDEX.close();

    fs::create_directory(folderToInit / ".ptitgit/index" / get_folder_to_object(C.get_hash_parent_tree()));
    fs::copy_file(folderToInit / ".ptitgit/objects" / get_path_to_object(C.get_hash_parent_tree()), folderToInit / ".ptitgit/index" / get_path_to_object(C.get_hash_parent_tree()));
    */
}
