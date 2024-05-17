#ifndef TCP_H
#define TCP_H

#include <string>

bool send_repos(int sockfd);
std::string receive_repos(int connection);

#endif
