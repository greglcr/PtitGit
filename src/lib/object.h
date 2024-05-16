#ifndef OBJECT_H
#define OBJECT_H

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string get_folder_to_object(std::string hashedContent);
std::string get_path_to_object(std::string hashedContent);

class Object  {

    public:

        std::string getContent();
        std::string getHashedContent();
        fs::path get_folder_to_write();
        fs::path getPathToWrite();
        void writeObject();

    protected:

        std::string content;
        std::string hashedContent;

};
#endif
