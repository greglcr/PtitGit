#include "hash_object.h"
#include "../lib/object.h"
#include "../lib/object_file.h"

#include <iostream>
#include <string>

std::string hash_object(File f) {

    return f.getHashedContent();

}



