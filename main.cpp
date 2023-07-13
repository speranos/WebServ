#include "webserv.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <set>


// void sendResponse(int clientSocket, const std::string& content, const std::string& contentType) {
// 	// std::cout << "\n\n" << content << "\n\n" << std::endl;
//     std::string response =
//         "HTTP/1.1 200 OK\r\n"
//         "Content-Type: " + contentType + "\r\n"
//         "Content-Length: " + std::to_string(content.length()) + "\r\n"
//         "Connection: close\r\n"
//         "\r\n" + content;

//     send(clientSocket, response.c_str(), response.length(), 0);
// }

// std::string readFile(const std::string& filename) {
//     std::ifstream file(filename);
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     return buffer.str();
// }

// void printFileDescriptors(const fd_set& fds) {
//     for (int fd = 0; fd < FD_SETSIZE; ++fd) {
//         if (FD_ISSET(fd, &fds)) {
//             std::cout << "File descriptor " << fd << " is set." << std::endl;
//         }
//     }
// }
void	add_to_map(int sck, Server_obj server, client &clt)
{
	std::pair<int, Server_obj>	pr;

	pr.first = sck;
	pr.second = server;
	clt.insert(pr);
}

int	ft_new_connex(int sck, std::set<int> &acceptedSockets, int MAX_FD, fd_set &read_fds, client &clt)
{
	int					acc_socket = 0;
	client::iterator	iter;

	if ((acc_socket = accept(sck, NULL, NULL)) > 0)
	{
		std::cout << "\nold_sock <<<<<< " << sck << std::endl;
		std::cout << "\nnew_sock <<<<<< " << acc_socket << std::endl;
		acceptedSockets.insert(acc_socket);
		if(MAX_FD <= acc_socket)
			MAX_FD = acc_socket + 1;
		FD_SET(acc_socket, &read_fds);
		iter = clt.find(sck);
		add_to_map(acc_socket, iter->second, clt);
		//To DO eares the old socket from the map
		// if(iter != clt.end())
		// 	clt.erase(sck);
		// (void)clt;
	}
	else
	{
		perror("accept failed");
		exit(0);
	}
	// (void)acceptedSockets;
	return(acc_socket);

}

int main(int ac, char **av)
{
	fd_set read_fds, tmp_fds, write_fds, accpted_fd;
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&accpted_fd);


	Server	server;
	client	clt;
	if(ac != 2)
	{
		std::cout << "ERROR: More or less then the argument requierd !" << std::endl;
		return(0);
	}
	
	try
	{
		ft_read(av[1], server);
		size_t	i = 0;
		int		sck;
		int		MAX_FD;
		int	sck_fd;
		sockaddr_in	address;
		while (i < server.size())
		{
			std::cout << "\n\nserver num :: " << i << "\n" << std::endl;
			sck_fd = ft_creat_sock(server[i], &address);
			FD_SET(sck_fd, &read_fds);
			add_to_map(sck_fd, server[i], clt);
			i++;
		}
		long	ret_read;
		std::set<int> acceptedSockets;
		MAX_FD = sck_fd + 1;
		sck = 0;
		char buffer[1024] = {0};

		while(1)
		{
			tmp_fds = read_fds;
			sck = 0;
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if(select(MAX_FD, &tmp_fds, NULL, NULL, NULL) < 0)
			{
				perror("select failed");
				exit(0);
			}
			while(sck < MAX_FD)
			{
				if(FD_ISSET(sck, &tmp_fds))
				{
					if(acceptedSockets.find(sck) == acceptedSockets.end())
					{
						sck = ft_new_connex(sck, acceptedSockets, MAX_FD, read_fds, clt);
					}
					//To Do
					//read
					//write
					break;
				}
				sck++;
				(void)write_fds;
			}

		ret_read = read( sck , buffer, 1024);
		std::cout << " request sie >>>>> " << ret_read << std::endl;
		printf("%s\n",buffer); //BUFFER_new IS THE REQUEST TO PARSS A KHAY SBA333333
		write(sck , "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!" , 74);
		printf("\n------------------Hello message sent-------------------\n");
		// after finishing reading set it to write_fds
		// after we send the response
		close(sck);
		FD_CLR(sck, &read_fds);
		acceptedSockets.erase(sck);
		// clear it from write_fds set
    }
}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}

// To DO:
//update the map after accepting a socket !