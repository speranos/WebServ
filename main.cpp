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
void	add_to_map(int sck, Server_obj &server, client &clt)
{
	std::pair<int, Server_obj>	pr;

	pr.first = sck;
	pr.second = server;
	clt.insert(pr);
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
	long	valread;
	std::set<int> acceptedSockets;
	MAX_FD = sck_fd + 1;
	int	acc_socket = 0;
	sck = 0;
	char buffer_old[1024] = {0};
	char buffer_new[1024] = {0};
	int	flag = 0;
    while(1)
    {
		tmp_fds = read_fds;
		sck = 0;
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if(select(MAX_FD + 1, &tmp_fds, NULL, NULL, NULL) < 0)
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
					if ((acc_socket = accept(sck, (struct sockaddr *)&address, (socklen_t*)&address.sin_len )) > 0)
					{
						std::cout << "acc_sck befor: " << acc_socket << std::endl;
						acceptedSockets.insert(acc_socket);
						if(MAX_FD <= acc_socket)
							MAX_FD = acc_socket + 1;
						FD_SET(acc_socket, &read_fds);
					}
					else
					{
						perror("accept failed");
						exit(0);
					}
				}
				else
				{
					valread = read(sck , buffer_old, 1024);
					printf("\n OLD CLIENT >>>>> \n");
					if(valread == 0)
					{
						close(sck);
						FD_CLR(sck, &read_fds);
					}
					printf("%s\n",buffer_old );
					std::string	request = buffer_old;
					flag = 1;
					write(sck , "hello" , strlen("hello"));
					printf("\n------------------Hello message sent-------------------\n");
				}
				break;
			}
			// if (FD_ISSET(sck, &tmp_fds) )
			// {

			// }
			// if(FD_ISSET(sck, &write_fds))
			// {
			// 	std::cout << "WRITE : " << sck << std::endl;
			// 	break;
			// }

			sck++;
			(void)write_fds;
		}
		if(flag == 0)
		{
			valread = read( acc_socket , buffer_new, 1024);
			printf("\nNEWWWWWW CLIENT >>>>> \n");
			std::cout << " request sie >>>>> " << valread << std::endl;
			printf("%s\n",buffer_new ); //BUFFER_new IS THE REQUEST TO PARSS A KHAY SBA333333
			std::string	request = buffer_new;
			write(acc_socket , "hello" , strlen("hello"));
			printf("\n------------------Hello message sent-------------------\n");
		}
    }
}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}

//add kola socketa ou address dyalha
//add kola socketa ou ou config data dyalha ou fd dyalha
//update fd mnin taccepta