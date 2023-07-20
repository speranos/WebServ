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

int	ft_new_connex(int sck, std::set<int> &acceptedSockets, int &MAX_FD, fd_set &read_fds, client &clt)
{
	int					acc_socket = 0;
	client::iterator	iter;
	Server_obj			tmp_serv;

	if ((acc_socket = accept(sck, NULL, NULL)) > 0)
	{
		std::cout << "\nold_sock <<<<<< " << sck << std::endl;
		std::cout << "\nnew_sock <<<<<< " << acc_socket << std::endl;
		acceptedSockets.insert(acc_socket);
		if(MAX_FD <= acc_socket)
			MAX_FD = acc_socket;
		FD_SET(acc_socket, &read_fds);

		iter = clt.find(sck);
		std::cout << "\n\nclient num ===== " << sck << std::endl;
		std::cout << "Host of the current client ==== " << iter->second.get_host() << "\n\n" << std::endl;

		tmp_serv = iter->second;

		// clt.erase(iter);
		add_to_map(acc_socket, tmp_serv, clt);
		//To DO eares the old socket from the map
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
	fd_set read_fds, read_master_fds, write_fds, write_master_fds, accpted_fd;
	FD_ZERO(&read_master_fds);
	FD_ZERO(&write_master_fds);
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
		int		sck_fd;
		sockaddr_in	address;
		while (i < server.size())
		{
			std::cout << "\n\nserver num :: " << i << "\n" << std::endl;
			sck_fd = ft_creat_sock(server[i], &address);
			FD_SET(sck_fd, &read_master_fds);
			add_to_map(sck_fd, server[i], clt);
			i++;
		}
		long	ret_read;
		std::set<int> acceptedSockets;
		MAX_FD = sck_fd;
		sck = 0;
		char buffer[1025] = {0};

		while(1)
		{
			read_fds = read_master_fds;
			write_fds = write_master_fds;
			sck = 0;
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if(select(MAX_FD + 1, &read_fds, &write_fds, NULL, NULL) < 0)
			{
				perror("select failed");
				exit(0);
			}
			while(sck <= MAX_FD)
			{
				if(FD_ISSET(sck, &read_fds))
				{
					if(acceptedSockets.find(sck) == acceptedSockets.end())
						sck = ft_new_connex(sck, acceptedSockets, MAX_FD, read_master_fds, clt);
					ret_read = read(sck , buffer, 1024);
					std::cout << " request sie >>>>> " << ret_read << std::endl;
					buffer[ret_read] = '\0';
					printf("%s\n",buffer); //BUFFER_new IS THE REQUEST TO PARSS A KHAY SBA333333
					// cast buffer to string
					// std::string str(buffer);
					std::cout << "to parsing >>>>>>>>>>>" <<  std::endl;
					std::string ft = buffer;
					pRequest(ft, clt, sck);
					if(ret_read < 1024)
					{
						std::cout << "send" << std::endl;
						FD_SET(sck, &write_master_fds);
						FD_CLR(sck, &read_master_fds);
					}
					break;
				}
				else if(FD_ISSET(sck, &write_fds))
				{
					write(sck , "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!" , 74);
					printf("\n------------------Hello message sent-------------------\n");
					std::cout << "sck eares >>>> " << sck << std::endl;
					close(sck);
					acceptedSockets.erase(sck);
					clt.erase(sck);
					FD_CLR(sck, &write_master_fds);
					break;
				}
				sck++;
			}

		// write(sck , "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!" , 74);
		// printf("\n------------------Hello message sent-------------------\n");
		// std::cout << "sck eares >>>> " << sck << std::endl;
		// close(sck);
		// FD_CLR(sck, &read_master_fds);
		// acceptedSockets.erase(sck);
		// clt.erase(sck);
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