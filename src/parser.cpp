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

    init();
    std::cout << 5 << std::endl;
    PtitGitRepos myRepos = PtitGitRepos();
    std::cout << 4 << std::endl;
    File myFile = File("test.txt");
    std::cout << 3 << std::endl;
    myRepos.writeObject(myFile);

}
    
