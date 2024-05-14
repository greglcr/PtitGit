#include "../lib/object.h"
#include "../lib/object_file.h"
#include "../lib/object_tree.h"
#include "../lib/repos.h"
#include "../lib/stagging_area.h"
#include "status.h"

#include <filesystem>


namespace fs = std::filesystem;

void status() {

    PtitGitRepos curRepos = PtitGitRepos();
    StaggingArea curStaggingArea = StaggingArea(curRepos);
    curStaggingArea.show_differences();

}

void status(fs::path pathToWorkingFolder) {

    PtitGitRepos curRepos = PtitGitRepos(pathToWorkingFolder);
    StaggingArea curStaggingArea = StaggingArea(curRepos);
    curStaggingArea.show_differences();

}