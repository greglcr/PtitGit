#include "../cmd/cat-object.h"
#include "../lib/repos.h"

#include <iostream>
#include <string>

std::string cat_object(std::string hashObject) {

    PtitGitRepos repos = PtitGitRepos();

    return repos.get_object_content(hashObject);

}