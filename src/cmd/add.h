#ifndef ADD_H
#define ADD_H

#include "../lib/repos.h"

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void add(std::vector<fs::path> listObject, PtitGitRepos curRepos);

#endif