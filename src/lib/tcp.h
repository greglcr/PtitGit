#ifndef TCP_H
#define TCP_H

#include <string>

int connect_to_server(std::string url, int port);

void send_message(int sockfd, std::string message);
std::string read_message(int sockfd, int size_max);

bool send_int(int sockfd, long val);
long read_int(int sockfd);

bool send_repos(int sockfd, std::string dir);
std::string receive_repos(int connection);

#endif
