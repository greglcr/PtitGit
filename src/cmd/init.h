#ifndef INIT_H
#define INIT_H

#include <filesystem>
#include <vector>

const std::vector<std::string> FOLDERS_TO_CREATE = {
            ".ptitgit",
            ".ptitgit/index",
            ".ptitgit/objects",
            ".ptitgit/objects/info",
            ".ptitgit/objects/pack",
            ".ptitgit/refs",
            ".ptitgit/refs/heads",
            ".ptitgit/refs/tags"
        };

void init();
void init(std::filesystem::path folderToInit);

/*
class PtiteGitInit {

    public:
        PtiteGitInit();
        PtiteGitInit(std::filesystem::path folderToInit);

    private:
        std::filesystem::path myGitFolder;

};

*/

#endif
