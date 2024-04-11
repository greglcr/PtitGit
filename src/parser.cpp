#include <iostream>
#include <string.h>
#include "cmd/init.h"
#include "cmd/hash_object.h"


int main(int argc,char *argv[])  {
    std::cout << argc << std::endl;

    if(strcmp(argv[1],"init")==0) init();
    if(strcmp(argv[1],"hash-object")==0) hash_object(File ());
}
    
