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

    protected:

        std::string content;
        std::string hashedContent;

};

#endif
