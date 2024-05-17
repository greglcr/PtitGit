#include "add.h"
#include "../lib/repos.h"
#include "../lib/stagging_area.h"

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void add(std::vector<fs::path> listObject, PtitGitRepos curRepos) {

}

void add() {

    PtitGitRepos curRepos = PtitGitRepos();
    StaggingArea curStagginArea = StaggingArea(curRepos);
    curStagginArea.add_all();

}