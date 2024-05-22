#ifndef COMPARE_REPOS_H
#define COMPARE_REPOS_H

#include <string>
#include "object_commit.h"

bool copy_all_objects(std::string src, std::string target);
std::string last_common_ancestor(Commit a, Commit b);


struct result_compare   {
    std::string branch_name;
    std::string commit_A;
    std::string commit_B;
    std::string commit_lca;
};

result_compare compare_branch(PtitGitRepos reposA, PtitGitRepos reposB);

#endif
