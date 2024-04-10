#include <iostream>
#include <string.h>
#include "cmd/init.h"


int main(int argc,char *argv[])  {
    std::cout << argc << std::endl;

    if(strcmp(argv[1],"init")==0) init();
}
    
