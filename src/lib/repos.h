#ifndef REPOS_H
#define REPOS_H

#include "object.h"
#include "object_file.h"
#include "object_tree.h"
#include "object_commit.h"

#include <filesystem>

namespace fs = std::filesystem;

class PtitGitRepos {

    public:

        PtitGitRepos();
        PtitGitRepos(fs::path workingFolder);

        void writeObject(Object myObjectToWrite);

    private:

        fs::path workingFolder;

};




#endif