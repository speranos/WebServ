#include "webserv.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <set>
#include "request/prequest.hpp"
#include "response.hpp"
#include <signal.h>
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
void	add_to_map(int sck, Server_obj server, client_config &clt)
{
	std::pair<int, Server_obj>	pr;

	pr.first = sck;
	pr.second = server;
	clt.insert(pr);
}

int	ft_new_connex(int sck, std::set<int> &acceptedSockets, int &MAX_FD, fd_set &read_fds, client_config &clt)
{
	int					acc_socket = 0;
	client_config::iterator	iter;
	Server_obj			tmp_serv;
	// new_client			client;

	if ((acc_socket = accept(sck, NULL, NULL)) > 0)
	{
		acceptedSockets.insert(acc_socket);
		if(MAX_FD <= acc_socket)
			MAX_FD = acc_socket;
		FD_SET(acc_socket, &read_fds);
		iter = clt.find(sck);
		tmp_serv = iter->second;
		add_to_map(acc_socket, tmp_serv, clt);
	}
	else
	{
		perror("accept failed"); 
		exit(0);
	}
	return(acc_socket);

}

void	ft_add_client(int sck, new_client &new_clt, request &rq, client &clt)
{
	clt.sett_rq_object(rq);
	// clt.sett_res_obj(res);
	std::pair<int, client>	pr;

	pr.first = sck;
	pr.second = clt;
	new_clt.insert(pr);

}

int main(int ac, char **av)
{
	fd_set read_fds, read_master_fds, write_fds, write_master_fds, accpted_fd;
	Server	server;
	client_config	clt_config;

	FD_ZERO(&read_master_fds);
	FD_ZERO(&write_master_fds);
	FD_ZERO(&accpted_fd);

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
		int it_sck;
		request req;
		request rq;
		sockaddr_in	address;
		new_client	new_clt;
		requests map;
		client		clt;

		while (i < server.size())
		{
			// std::cout << "\n\nserver num :: " << i << "\n" << std::endl;
			sck_fd = ft_creat_sock(server[i], &address);
			std::cout << "setting sock " << sck_fd <<std::endl;
			FD_SET(sck_fd, &read_master_fds);
			add_to_map(sck_fd, server[i], clt_config);
			i++;
		}
		long	ret_read;
		std::set<int> acceptedSockets;
		MAX_FD = sck_fd;
		sck = 0;
		// char buffer[1025] = {0};
		std::string buffer;
		buffer.resize(1024);
		// signal(SIGPIPE, SIG_IGN);

		while(1)
		{
			read_fds = read_master_fds;
			write_fds = write_master_fds;
			sck = 0;
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if(select(MAX_FD + 1, &read_fds, &write_fds, NULL, NULL) < 0)
			{
				std::cout << "reading from " << sck << std::endl;
				perror("select failed");
				exit(0);
			}

			while(sck <= MAX_FD)
			{

				if(FD_ISSET(sck, &read_fds))
				{

					if(acceptedSockets.find(sck) == acceptedSockets.end())
						sck = ft_new_connex(sck, acceptedSockets, MAX_FD, read_master_fds, clt_config);
					ret_read = read(sck , (void *)buffer.c_str(), 1024);

					std::cout << buffer << std::endl;
					rq = pRequest(buffer, clt_config, sck, map);
					ft_add_client(sck, new_clt, rq, clt);
					new_client::iterator it;
					it = new_clt.find(sck);
					it->second.sett_rq_object(rq);
					std::cout << "it->forst >>>>>>>"<< it->first << std::endl;
					it_sck = it->first;
					// std::cout << "client sck >>>>>>>>>>> " << it->first << std::endl;
					req = it->second.get_rq_object();
					std::map<std::string, std::string> headers = req.getHeaders();
					// for(std::map<std::string, std::string>::iterator ita = headers.begin(); ita != headers.end(); ita++)
					// {
					// 	std::cout << ita->first << " : " << ita->second << std::endl;
					// }
					req._res = new response();

					// req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
					// req._res->set_get_con_type(req);
					// req._res->setContentLenght(req);
					if(req.getMethod() == "GET")
						req._res->GetMethod(req);
					buffer.clear();
					buffer.resize(1024);
					if(ret_read < 1024)
					{
						std::cout << "ret read >>> " << ret_read << std::endl;
						FD_SET(it_sck, &write_master_fds);
						FD_CLR(it_sck, &read_master_fds);
					}
					break;
				}
				else if(FD_ISSET(it_sck, &write_fds))
				{
					req._res->Send(it_sck, req);

					printf("\n------------------Hello message sent-------------------\n");
					if(req._res->_isDone == true)
					 {
						std::cout << "sck erase >>>> " << it_sck << std::endl;
						// std::cout << " . aaaaaaaaa" << std::endl;
						FD_CLR(it_sck, &write_master_fds);
						close(it_sck);
						map.erase(it_sck);
						new_clt.erase(it_sck);
						acceptedSockets.erase(it_sck);
						clt_config.erase(it_sck);
					 }
					break;
				}
				sck++;
			}
    }
}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}

// To DO:
//update the map after accepting a socket !