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
    // read until '\0'
    char cara [1];
    bool ok = true;
    std::string result = "";
    while (ok)  {
        int r = read(sockfd, cara, 1);
        if (r != 1 || cara[0] == '\0')
            ok = false;
        else
            result += cara[0];
    }
    return result;
}

bool send_int(int sockfd, long val) {
    unsigned char bytes[5];
    bytes[0] = (val >> 24) & 0xFF;
    bytes[1] = (val >> 16) & 0xFF;
    bytes[2] = (val >> 8) & 0xFF;
    bytes[3] = val & 0xFF;
    bytes[4] = '\0';
    if (write(sockfd, bytes, 5)==0)    {
        std::cerr << "Can not send value" << std::endl;
        return false;
    }
    return true;
}
long read_int(int sockfd)   {
    char buffer[5];
    read(sockfd, buffer, 5);
    long result = ((long)buffer[3]&0xFF) + (((long)buffer[2]&0xFF) << 8) + (((long)buffer[1]&0xFF) << 16) + (((long)buffer[0]&0xFF) << 24);
    return result;
}


bool send_repos(int sockfd, std::string repos)    {
    // store the .ptitgit in a tgz archive
    char name_tgz [L_tmpnam];
    std::tmpnam(name_tgz);
    std::string name_tgz_str (name_tgz);
    system(("tar -czf "+name_tgz_str+" -C "+repos+" .ptitgit").c_str());
    
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
    if (!send_int(sockfd, size_tar)) return false;

    // then we send the content
    if (write(sockfd, tarContent, size_tar+1) == 0) {
        std::cerr << "Can not send data" << std::endl;
        return false;
    }
    return true;
}


std::string receive_repos(int sockfd)    {
    // read the tar size
    long size_tar = read_int(sockfd);

    // read the tar
    char tarContent [size_tar];
    read(sockfd, tarContent, size_tar);

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
    char *name_directory = mkdtemp(strcpy(templatebuf, "/tmp/ptitgitXXXXXX"));
    std::string name_directory_str (name_directory);
    std::cout << "repos received in directory : " << name_directory << std::endl;
    
    // extract into the directory
    system(("tar -xf " + name_tgz_str + " -C " + name_directory_str).c_str());
    remove(name_tgz);

    return name_directory_str;
}
