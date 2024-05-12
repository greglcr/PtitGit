#include "object.h"

#include <filesystem>
#include <iterator>
#include <string>

namespace fs = std::filesystem;

std::string Object::getContent() {

    return this->content;

}

std::string Object::getHashedContent() {

    return this->hashedContent;

}

fs::path Object::getPathToWrite() {

    return get_path_to_object(this->hashedContent);

}

std::string get_path_to_object(std::string hashedContent) {

    std::string::iterator beginContent = hashedContent.begin();
    std::string::iterator endContent = hashedContent.end();
    
    fs::path folder = std::string(beginContent, beginContent + 2);
    fs::path fileName = std::string(beginContent + 2, endContent);

    return folder / fileName;

}

