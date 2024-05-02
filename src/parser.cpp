#include "cmd/cat-object.h"
#include "cmd/init.h"
#include "cmd/hash_object.h"
#include "lib/hashing.h"
#include "lib/object.h"
#include "lib/object_file.h"
#include "lib/object_tree.h"
#include "lib/repos.h"

#include <iostream>
#include <string.h>

int main(int argc,char *argv[])  {

    if (argc > 1 && strcmp(argv[1],"init") == 0) {
        if (argc > 2) {
            init(argv[2]);
        }
        else {
            init();
        }
    }
    else if(argc > 2 && strcmp(argv[1], "hash-object") == 0) {
        if (argc < 3) {
            std::cerr << "Error : Object to hash missing" << std::endl;
        }
        else {
            std::cout << hash_object(File(argv[2])) << std::endl; 
        }
    }
    else if (argc > 2 && strcmp(argv[1], "cat-file") == 0) {
        if(argc < 3) {
            std::cerr << "Error : Object to display missing" << std::endl;
        }
        else {
            std::cout << cat_object(argv[2]) << std::endl;
        }
    }

}
    
