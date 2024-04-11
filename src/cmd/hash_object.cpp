#include "hash_object.h"
#include "../lib/object.h"
#include <iostream>

void hash_object(File f)    {
    std::cout << f.hash() << std::endl;
}



