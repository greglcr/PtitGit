#ifndef OBJECT_H
#define OBJECT_H

#include "object.h"

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class Object  {

    std::string hash();

};

class Commit : public Object {

    public:

    private:


};

#endif
