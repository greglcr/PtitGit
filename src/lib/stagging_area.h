#ifndef STAGGING_AREA_H
#define STAGGING_AREA_H

#include "repos.h"

#include <map>
#include <vector>

class StaggingArea {

    public:

        StaggingArea(PtitGitRepos repos);
        void construct_tree(std::string curFileHash);
        void show_differences();
        void show_differences(fs::path curPosWorkingArea, std::string curHashStaggingArea);

    private:

        PtitGitRepos repos;
        std::string rootTree;
        std::map<std::pair<std::string, fs::path> , std::vector<std::string> > treeStaggingArea; //Need to store the file path to check if there is a creation of something

};

#endif