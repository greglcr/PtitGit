#ifndef OBJECT_H
#define OBJECT_H

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string get_path_to_object(std::string hashedContent);

class Object  {

    public:

        std::string getContent();
        std::string getHashedContent();
        fs::path getPathToWrite();
        void fromstring(std::string);
    protected:

        std::string content;
        std::string hashedContent;

};

#endif
