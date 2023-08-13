#include "socket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>

int	ft_creat_sock(Server_obj &server, sockaddr_in *addr)
{
	int			sck_fd;
	int			Port = atoi(server.get_port().c_str());

	std::string	host_std = server.get_host();
	std::cout << "host === " << host_std << std::endl;
	std::cout << "Port === " << Port << std::endl;
	const char	*host = host_std.c_str();

	addr->sin_family = AF_INET;
	addr->sin_port = htons(Port);
	addr->sin_addr.s_addr = inet_addr(host);
	
	sck_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	if (setsockopt(sck_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 || Port < 0) {
		std::cout << "setsockopt" << std::endl;
		exit(0);
	}
	if((bind(sck_fd, (sockaddr*)addr, sizeof(sockaddr_in))) < 0) {
		perror("Bind failed");
		exit(0);
	}
	if(listen(sck_fd, FD_SETSIZE) < 0)
	{
		std::cout << "listen Failed" << std::endl;
		exit(0);	
	}
	return(sck_fd);
}