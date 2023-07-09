#include "webserv.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <set>


void sendResponse(int clientSocket, const std::string& content, const std::string& contentType) {
	// std::cout << "\n\n" << content << "\n\n" << std::endl;
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " + contentType + "\r\n"
        "Content-Length: " + std::to_string(content.length()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + content;

    send(clientSocket, response.c_str(), response.length(), 0);
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printFileDescriptors(const fd_set& fds) {
    for (int fd = 0; fd < FD_SETSIZE; ++fd) {
        if (FD_ISSET(fd, &fds)) {
            std::cout << "File descriptor " << fd << " is set." << std::endl;
        }
    }
}

int main(int ac, char **av)
{
	fd_set read_fds, tmp_fds, write_fds, accpted_fd;
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&accpted_fd);


	Server	server;
	if(ac != 2)
	{
		std::cout << "ERROR: More or less then the argument requierd !" << std::endl;
		return(0);
	}
	
	try
	{
		ft_read(av[1], server);
		// getaddrinfo();
		//create multi socket
		//create a structure to handle reading for multiple fd
		//handle non-blocking socket
		//check for host and port if entred twice
		size_t	i = 0;
		int		sck;
		int		MAX_FD;
		int	sck_fd;
		sockaddr_in	address;
		while (i < server.size())
		{
			std::cout << "\n\nserver num :: " << i << "\n" << std::endl;
			// server[i++].getter();
			sck_fd = ft_creat_sock(server[i], &address);
			FD_SET(sck_fd, &read_fds);
			i++;
		}
	long	valread;
	std::set<int> acceptedSockets;
	MAX_FD = sck_fd;
	int	acc_socket = 0;
	sck = 0;
    while(1)
    {
		tmp_fds = read_fds;
		sck =  FD_SETSIZE + 1;
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if(select(MAX_FD + 1, &tmp_fds, NULL, NULL, NULL) < 0)
		{
			perror("select failed");
			exit(0);
		}
		while(sck <= MAX_FD)
		{
			if(FD_ISSET(sck, &tmp_fds))
			{
				if(acceptedSockets.find(acc_socket) == acceptedSockets.end())
				{
					std::cout << "READ: " << sck << std::endl;
					if ((acc_socket = accept(sck, (struct sockaddr *)&address, (socklen_t*)&address.sin_len )) > 0)
					{
						acceptedSockets.insert(acc_socket);
						if(MAX_FD <= acc_socket)
							MAX_FD = acc_socket;
						FD_SET(acc_socket, &read_fds);
					}
					else
					{
						perror("accept failed");
						exit(0);
					}
				}
				break;
			}
			(void)write_fds;
			// else if(FD_ISSET(sck, &write_fds))
			// {
			// 	std::cout << "WRITE : " << sck << std::endl;
			// 	break;	
			// }
			sck++;
		}
		char buffer[30000] = {0};
		valread = read( acc_socket , buffer, 30000);
		printf("%s\n",buffer ); //BUFFER IS THE REQUEST TO PARSS A KHAY SBA333333
		std::string	request = buffer;
		if (request.find(".css") != std::string::npos)
			sendResponse(acc_socket, readFile("responsive-css-grid-nike-layout/dist/style.css"), "text/css");
		else
			sendResponse(acc_socket, readFile("responsive-css-grid-nike-layout/dist/index.html"), "text/html");
		printf("------------------Hello message sent-------------------");
		// break;
		close(acc_socket);
    }
	(void)valread;
}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}

//add kola socketa ou address dyalha
//add kola socketa ou ou config data dyalha ou fd dyalha
//update fd mnin taccepta