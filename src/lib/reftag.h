#ifndef REFTAG_H
#define REFTAG_H

#include <map>
#include "repos.h"
#include "object.h"
#include "object_commit.h"
#include<iostream>

namespace fs = std::filesystem;

std::string ref_resolve(PtitGitRepos X, fs::path path);
std::map <fs::path, std::string> ref_list_basic(PtitGitRepos X);
std::map <fs::path, std::string> ref_list(std::map <fs::path, std::string> current, PtitGitRepos X, fs::path path);

class Tag : public Object {
    public:
        Tag(Object tagObject, std::string tagAuthor = "", std::string tagger ="PtiteGit Team", std::string tagName="abc", std::string tagMessage = "New tag");

    private:
    Object tagObject;
    std::string tagAuthor;
    std::string tagger;
    std::string tagName;
    std::string tagMessage;
};
//void tag_create(PtitGitRepos X, std::string tag_name, std::string tagged_object, bool create=false);
#endif