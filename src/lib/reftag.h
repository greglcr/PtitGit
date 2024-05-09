#ifndef REFTAG_H
#define REFTAG_H

#include <map>
#include "repos.h"
#include "object.h"
#include<iostream>

namespace fs = std::filesystem;

std::string ref_resolve(PtitGitRepos X, fs::path path);
std::map <fs::path, std::string> ref_list_basic(PtitGitRepos X);
std::map <fs::path, std::string> ref_list(PtitGitRepos X, fs::path path=".", std::map <fs::path, std::string> current);
#endif