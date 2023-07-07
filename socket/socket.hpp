#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../webserv.hpp"
#include "../config_file/config_file.hpp"



int	ft_creat_sock(Server_obj &server, sockaddr_in *addr);


#endif