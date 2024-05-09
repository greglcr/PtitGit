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
#include <vector>

int edit_distance(std::string a, std::string b) {
    if (a == b) return 0;

    int n = a.length();
    int m = b.length();
    if (n == 0)  return m;
    if (m == 0)  return n;

    std::string aBis = a.substr(0, n-1);
    std::string bBis = b.substr(0, m-1);

    if (a[n-1] == b[m-1])   return edit_distance(aBis, bBis);
    return 1 + std::min( edit_distance(aBis, bBis), std::min(edit_distance(aBis, b), edit_distance(a, bBis)));
}

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
    else    {
        std::cerr << "'" << argv[1] << "' is not a command." << std::endl;
        std::vector<std::string> lst_commands { "init", "hash-object", "cat-file" };

        int dist_min = 100000000;
        std::string command_min = "???????";
        for (auto cmd : lst_commands)   {
            int dist = edit_distance(cmd, argv[1]);
            if (dist <= dist_min)   {
                dist_min = dist;
                command_min = cmd;
            }
        }
        std::cerr << "Maybe you want to use :    " << command_min << std::endl;
    }

}
    
