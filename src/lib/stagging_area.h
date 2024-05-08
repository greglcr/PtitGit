#ifndef STAGGING_AREA_H
#define STAGGING_AREA_H

#include "repos.h"

#include <map>
#include <vector>

class StaggingArea {

    public:

        StaggingArea(PtitGitRepos repos);

    private:

        PtitGitRepos repos;
        std::string rootTree;
        std::map<std::string, std::vector<std::string> > treeStaggingArea;

        void construct_tree(std::string curFileHash);

};

#endif