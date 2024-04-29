#ifndef OBJECT_COMMIT_H
#define OBJECT_COMMIT_H

#include "object.h"
#include "repos.h"
#include "object_tree.h"

#include <vector>

class Commit : public Object {

    public:

        Commit();

    private:

        std::vector<Commit> parentCommits;
        std::string commitAuthor;
        Tree parentTree;
        //PtitGitRepos repos;

};

#endif