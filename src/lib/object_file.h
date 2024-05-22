#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include "object.h"
#include "repos.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class File : public Object  {

    public:

        File(fs::path filePath = ".", bool create = false);
        File createFileFromContent(std::string, bool = false, PtitGitRepos repos = PtitGitRepos());
        void updateContent();
        fs::path get_file_path();
        std::string getActualFile();
    private:
        std::string actualFile;
        fs::path filePath;

};
File findFile(std::string, bool create = false, PtitGitRepos repos = PtitGitRepos());
bool compareFile(File,File);
#endif
