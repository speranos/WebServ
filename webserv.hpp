#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
// #include "response.hpp"

#include "config_file/config_file.hpp"
typedef std::map<int, Server_obj> client;
#include "socket/socket.hpp"
// #include "request/prequest.hpp"

//void    GetMethod(location_obj &location,request &req);
#endif