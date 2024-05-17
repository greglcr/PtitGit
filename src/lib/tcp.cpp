#include "tcp.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <sys/stat.h>

bool send_repos(int sockfd)    {
    // store the .ptitgit in a tgz archive
    char name_tgz [L_tmpnam];
    std::tmpnam(name_tgz);
    std::string name_tgz_str (name_tgz);
    system(("tar -czf "+name_tgz_str+" .ptitgit").c_str());

    
    // read this file
    FILE *tar;
    tar = fopen(name_tgz, "rb"); 

    if (fseek(tar, 0L, SEEK_END))   {
        std::cerr << "Unable to move in the binary" << std::endl;
        return false;
    }
    unsigned long size_tar = ftell(tar);
    if (fseek(tar, 0L, SEEK_SET))   {
        std::cerr << "Unable to move in the binary" << std::endl;
        return false;
    }

    char tarContent [size_tar];
    if (fread(tarContent, 1, size_tar, tar) != size_tar) {
        std::cerr << "Unable do read the tar archive" << std::endl;
        return false;
    }
    fclose(tar);

    // first, we send the size :
    unsigned char size_tar_bytes[5];
    size_tar_bytes[0] = (size_tar >> 24) & 0xFF;
    size_tar_bytes[1] = (size_tar >> 16) & 0xFF;
    size_tar_bytes[2] = (size_tar >> 8) & 0xFF;
    size_tar_bytes[3] = size_tar & 0xFF;
    size_tar_bytes[4] = '\0';

    if (write(sockfd, size_tar_bytes, 5)==0)    {
        std::cerr << "Can not send data" << std::endl;
        return false;
    }

    // then we send the content
    if (write(sockfd, tarContent, size_tar+1) == 0) {
        std::cerr << "Can not send data" << std::endl;
        return false;
    }
    return true;
}


std::string receive_repos(int connection)    {
    // read the tar size
    char buffer_size[5];
    read(connection, buffer_size, 5);
    long size_tar = buffer_size[3] + ((long)buffer_size[2] << 8) + ((long)buffer_size[1] << 16) + ((long)buffer_size[0] << 24);

    // read the tar
    char tarContent [size_tar];
    read(connection, tarContent, size_tar);

    // write it
    char name_tgz [L_tmpnam];
    std::tmpnam(name_tgz);
    std::string name_tgz_str (name_tgz);

    FILE* tar;
    tar = fopen(name_tgz, "wb");
    fwrite(tarContent, 1, size_tar, tar);
    fclose(tar);
    
    // create a directory to store the repos
    
    char templatebuf[80];
    char *name_directory = mkdtemp(strcpy(templatebuf, "/tmp/ptitgitXXXXXXXX"));
    std::string name_directory_str (name_directory);
    std::cout << "repos received in directory : " << name_directory << std::endl;
    
    // extract into the directory
    system(("tar -xf " + name_tgz_str + " -C " + name_directory_str).c_str());
    remove(name_tgz);

    return name_directory_str;
}
