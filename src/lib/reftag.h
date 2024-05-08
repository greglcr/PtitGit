#ifndef REFTAG_H
#define REFTAG_H

#include "repos.h"
#include "object.h"
#include<iostream>

namespace fs = std::filesystem;

std::string ref_resolve(PtitGitRepos X, fs::path path);

#endif