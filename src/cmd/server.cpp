#include "server.h"
#include <iostream>
#include <unistd.h>

void server()   {
    while (true)    {
        std::cout << "server is running" << std::endl;
        usleep(1000000);
    }
}
