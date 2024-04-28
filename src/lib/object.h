#ifndef OBJECT_H
#define OBJECT_H

#include "object.h"

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Object  {

    public:

        std::string getContent();
        std::string getHashedContent();
        fs::path getPathToWrite();

    protected:

        std::string content;
        std::string hashedContent;

};

#endif
