#include "../lib/object.h"
#include "../lib/object_file.h"
#include "../lib/object_tree.h"
#include "../lib/repos.h"
#include "status.h"

#include <filesystem>


namespace fs = std::filesystem;

void status(PtitGitRepos curRepos) {

    /* The folder index will store all the information about the stagging area. Basically, it will have a object folder, like the other folder folder, that will store
    all the version of the new files. When a git commit is computed, it will move all the useful files to the real object area, and it will then erase all the content of
    the index folder */

}