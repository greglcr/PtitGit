#ifndef OBJECT_COMMIT_H
#define OBJECT_COMMIT_H

#include "object.h"
#include "repos.h"
#include "object_tree.h"

#include <vector>

class Commit : public Object {

    public:

        Commit(Tree parentTree, std::vector<std::string> parentCommitsHash = {}, std::string commitAuthor = "", std::string committer ="PtiteGit Team", std::string message="New commit", std::string gpgsig="");
        Commit();
        Commit fromstring(std::string, PtitGitRepos repos = PtitGitRepos());
        Commit fromfile(std::string, PtitGitRepos repos = PtitGitRepos ());
        void calculateContent();
        Tree getTree();
        std::string get_hash_parent_tree();
        std::vector<std::string> get_parents_hash();
        //void writeCommit();
    private:

        std::vector<std::string> parentCommitsHash;
        std::string commitAuthor;
        std::string committer;
        std::string message;
        std::string gpgsig;
        Tree parentTree;
        //PtitGitRepos repos;
};

void checkout(std::string,fs::path,bool force = false);
void tree_checkout(Tree,fs::path,bool force = false);
void INDEXreset(Commit);
#endif
