#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include "object.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class File : public Object  {

    public:

        File(fs::path filePath);
        
        void updateContent();
        void printContent();
        std::string getHash();

    private:

        fs::path filePath;
        std::string content;
        std::string hashedContent;

};

#endif