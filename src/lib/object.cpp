#include "object.h"
#include <string>
#include "hashing.h"



std::string File::hash()    {
    std::string toHash = "file:" + path + ":" + content;
    return hashString(toHash);
}
