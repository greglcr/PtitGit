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
#include <string>

int connect_to_server(std::string url, int port) {
    struct sockaddr_in serv_addr;
    struct hostent *server;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "ERROR when opening the socket" << std::endl;
        exit(-1);
    }
    server = gethostbyname(url.c_str());
    if (server == NULL) {
        std::cerr << "ERROR : no such host" << std::endl;
        exit(-1);
    }


    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,  server->h_length);

    serv_addr.sin_port = htons(port);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "ERROR connecting" << std::endl;
        exit(-1);
    }
    return sockfd;
}

void send_message(int sockfd, std::string message)  {
    send(sockfd, message.c_str(), message.size()+1, 0);
}
std::string read_message(int sockfd, int size_max)  {
    char result [size_max+1];
    bzero(result, size_max+1);
    read(sockfd, result, size_max);
    std::string s (result);
    return s;
}

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
    long size_tar = ((long)buffer_size[3]&0xFF) + (((long)buffer_size[2]&0xFF) << 8) + (((long)buffer_size[1]&0xFF) << 16) + (((long)buffer_size[0]&0xFF) << 24);

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
