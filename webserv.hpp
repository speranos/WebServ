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


#include "config_file/config_file.hpp"
typedef std::map<int, Server_obj> client_config;
#include "socket/socket.hpp"
#include "request/prequest.hpp"
#include "client.hpp"
// typedef std::map<int, Server_obj> client_config;

// #include "response.hpp"
// class request;
// class response;

// class client
// {
//     request rq_obj;
//     response res_obj;
//     public:
//     void    sett_rq_object(request rq_obj){this->rq_obj = rq_obj;};
//     // void    sett_res_obj(response res_obj){this->res_obj = res_obj;};
//     request &get_rq_object(){return(rq_obj);};
//     response    &get_res_obj(){return(res_obj);};
// };
// typedef std::map<int, client> new_client;
//void    GetMethod(location_obj &location,request &req);

#endif
