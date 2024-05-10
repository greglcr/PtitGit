#ifndef REPOS_H
#define REPOS_H

#include "object.h"
#include "object_file.h"
#include "object_tree.h"
#include "object_commit.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class PtitGitRepos {

    public:

        PtitGitRepos();
        PtitGitRepos(fs::path workingFolder);

        void writeObject(Object myObjectToWrite);
        std::string get_object_content(std::string hashedObject);

        fs::path getWorkingFolder();

        std::string get_config(std::string key);
        void set_config(std::string key, std::string value);
    

    private:

        fs::path workingFolder;

};




#endif
