#ifndef OBJECT_H
#define OBJECT_H
#include "object.h"
#include <string>


class Object  {
    std::string hash();

};

class File : public Object  {
    std::string path;
    std::string content;
    
    public:
    std::string hash();
};

#endif
