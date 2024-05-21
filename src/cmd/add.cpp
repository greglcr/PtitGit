#include "add.h"
#include "../lib/repos.h"
#include "../lib/stagging_area.h"

#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

void add(fs::path pathToAdd) {

    PtitGitRepos curRepos = PtitGitRepos();
    StaggingArea curStagginArea = StaggingArea(curRepos);
    curStagginArea.add(pathToAdd);
}

void add() {

    PtitGitRepos curRepos = PtitGitRepos();
    StaggingArea curStagginArea = StaggingArea(curRepos);
    curStagginArea.add_all();

}