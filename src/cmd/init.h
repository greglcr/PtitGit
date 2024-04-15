#ifndef INIT_H
#define INIT_H

#include <filesystem>

void init();
void init(std::filesystem::path folderToInit);

class PtiteGitInit {

    public:
        PtiteGitInit();
        PtiteGitInit(std::filesystem::path folderToInit);

    private:
        std::filesystem::path myGitFolder;
};

#endif
