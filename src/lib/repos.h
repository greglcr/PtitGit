#ifndef REPOS_H
#define REPOS_H

#include <filesystem>

namespace fs = std::filesystem;

class PtitGitRepos {

    public:

        PtitGitRepos();
        PtitGitRepos(fs::path workingFolder);

    private:

        fs::path workingFolder;

};




#endif