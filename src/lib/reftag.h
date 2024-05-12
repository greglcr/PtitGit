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
        Tag(Object tagObject = Commit(), std::string tagType = "commit", std::string tagAuthor = "", std::string tagger ="PtiteGit Team", std::string tagName="abc", std::string tagMessage = "New tag");
        void tag_create(PtitGitRepos X, std::string tag_name, std::string tagged_object, std::string tag_message, bool create=false);
        void calculateContent();
        Tag fromfile(std::string);
        Tag fromstring(std::string);
        void writeTag();
    private:
    Object tagObject;
    std::string tagAuthor;
    std::string tagger;
    std::string tagName;
    std::string tagMessage;
    std::string tagType;
};

#endif