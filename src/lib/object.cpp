#include "object.h"

#include <string>

std::string Object::getContent() {

    return this->content;

}

std::string Object::getHashedContent() {

    return this->hashedContent;

}