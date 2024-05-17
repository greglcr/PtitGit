#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include "object.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class File : public Object  {

    public:

        File(fs::path filePath, bool create = false);

        void updateContent();
        fs::path get_file_path();
        
    private:

        fs::path filePath;

};
File findFile(std::string,bool create = false);
#endif