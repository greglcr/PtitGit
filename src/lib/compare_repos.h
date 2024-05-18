#ifndef COMPARE_REPOS_H
#define COMPARE_REPOS_H

#include <string>
#include "object_commit.h"

bool copy_all_objects(std::string src, std::string target);
Commit last_common_ancestor(Commit a, Commit b);

#endif
