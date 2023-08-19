#include "webserv.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <set>
#include "request/prequest.hpp"
#include "response.hpp"
#include <signal.h>

void add_to_map(int sck, Server_obj server, client_config &clt)
{
	std::pair<int, Server_obj> pr;

	pr.first = sck;
	pr.second = server;
	clt.insert(pr);
}

int ft_new_connex(int sck, std::set<int> &acceptedSockets, int &MAX_FD, fd_set &read_fds, client_config &clt)
{
	int acc_socket = 0;
	client_config::iterator iter;
	Server_obj tmp_serv;

	if ((acc_socket = accept(sck, NULL, NULL)) > 0)
	{
		acceptedSockets.insert(acc_socket);
		if (MAX_FD <= acc_socket)
			MAX_FD = acc_socket;
		FD_SET(acc_socket, &read_fds);
		iter = clt.find(sck);
		tmp_serv = iter->second;
		add_to_map(acc_socket, tmp_serv, clt);
	}
	else
	{
		std::cout << "ERHWRETJETYJ  " <<acc_socket << std::endl;
		perror("accept failed");
		exit(0);
	}
	return (acc_socket);
}

void ft_add_client(int sck, new_client &new_clt, request &rq, client &clt)
{
	clt.sett_rq_object(rq);
	std::pair<int, client> pr;

	pr.first = sck;
	pr.second = clt;
	new_clt.insert(pr);
}

int main(int ac, char **av)
{
	fd_set read_fds, read_master_fds, write_fds, write_master_fds, accpted_fd;
	Server server;
	client_config clt_config;

	FD_ZERO(&read_master_fds);
	FD_ZERO(&write_master_fds);
	FD_ZERO(&accpted_fd);

	if (ac != 2)
	{
		std::cout << "ERROR: More or less then the argument requierd !" << std::endl;
		return (0);
	}

	try
	{
		ft_read(av[1], server);
		size_t i = 0;
		int sck;
		int MAX_FD;
		int sck_fd;
		int it_sck = 0;
		request req;
		request rq;
		sockaddr_in address;
		new_client new_clt;
		requests map;
		client clt;

		while (i < server.size())
		{
			sck_fd = ft_creat_sock(server[i], &address);
			FD_SET(sck_fd, &read_master_fds);
			add_to_map(sck_fd, server[i], clt_config);
			i++;
		}
		long ret_read;
		std::set<int> acceptedSockets;
		MAX_FD = sck_fd;
		sck = 0;
		std::string buffer;
		buffer.resize(1024);

		while (1)
		{
			read_fds = read_master_fds;
			write_fds = write_master_fds;
			sck = 0;
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if (select(MAX_FD + 1, &read_fds, &write_fds, NULL, NULL) < 0)
			{
				perror("select failed");
				exit(0);
			}

			while (sck <= MAX_FD)
			{

				if (FD_ISSET(sck, &read_fds))
				{

					if (acceptedSockets.find(sck) == acceptedSockets.end())
						sck = ft_new_connex(sck, acceptedSockets, MAX_FD, read_master_fds, clt_config);
					ret_read = read(sck, (void *)buffer.c_str(), 1024);
					rq = pRequest(buffer, clt_config, sck, map, ret_read);
					ft_add_client(sck, new_clt, rq, clt);
					new_client::iterator it;
					it = new_clt.find(sck);
					it->second.sett_rq_object(rq);
					it_sck = it->first;
					map[it_sck] = it->second.get_rq_object();
					client_config::iterator iter;
					iter = clt_config.find(it_sck);
					map[it_sck].port =  iter->second.get_port(); 
					std::map<std::string, std::string> headers = map[it_sck].getHeaders();
					map[it_sck]._res = new response();
					
					if (map[it_sck].getMethod() == "GET")
					{
						std::cout << req.done << std::endl;
						if (!map[it_sck]._loc.get_redir().empty())
						{
							map[it_sck].op = 5;
						}
						else
							map[it_sck]._res->GetMethod(map[it_sck]);
					}
					else if ((map[it_sck].getMethod() == "POST"))
					{
						if (!map[it_sck]._loc.get_redir().empty())
						{
							map[it_sck].op = 5;
						}
						if (map[it_sck].getIsDone() == true){
							map[it_sck]._res->MethodPost(map[it_sck]);
						}
					}
					else if (map[it_sck].getMethod() == "DELETE")
						map[it_sck]._res->methodDelete(req);
					buffer.clear();
					buffer.resize(1024);
					if (map[it_sck].getIsDone() == true)
					{
						FD_SET(it_sck, &write_master_fds);
						FD_CLR(it_sck, &read_master_fds);
					}
					break;
				}
				else if (FD_ISSET(sck, &write_fds))
				{
					printf("\n------------------Hello message sent-------------------\n");
					map[sck]._res->Send(sck, map[sck]);
					if (map[sck]._isDone == true)
					{
						// std::cout << "sck erase >>>> " << sck << std::endl;
						FD_CLR(sck, &write_master_fds);
						close(sck);
						map.erase(sck);
						new_clt.erase(sck);
						acceptedSockets.erase(sck);
						clt_config.erase(sck);
					}
					break;
				}
				sck++;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
