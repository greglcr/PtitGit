#include "add.h"
#include "../lib/repos.h"
#include "../lib/stagging_area.h"

#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

void add(fs::path fileToAdd) {

    if (fs::is_directory(fileToAdd)) {
        std::cerr << "Error : Only file adding is supported for the moment, use ./petitgit add to add everything" << std::endl;
        exit(0);
    }

    PtitGitRepos curRepos = PtitGitRepos();
    StaggingArea curStagginArea = StaggingArea(curRepos);
    curStagginArea.add(fileToAdd);
}

void add() {

    PtitGitRepos curRepos = PtitGitRepos();
    StaggingArea curStagginArea = StaggingArea(curRepos);
    curStagginArea.add_all();

}