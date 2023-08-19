#include "../webserv.hpp"

void	ft_read(char *file_name, Server &config_file)
{
	std::fstream	file;

	file.open(file_name);
	if(file.fail() || file.eof())
		throw std::invalid_argument("ERROR: File not found !");
	while(!file.eof())
	{
		ft_add_server(file, config_file);
	}
	final_check(config_file);
}

void	final_check(Server &config_file)
{
	Server::iterator iter = config_file.begin();
	typedef std::vector<location_obj> location;
	location::iterator iter_loc;
	location tmp_loc;
	while(iter != config_file.end())
	{
		if(iter->get_host().size() == 0 || iter->get_port().size() == 0 || iter->get_location().size() == 0)
			throw std::invalid_argument("ERROR: missing some info !");
		if(iter->getBodySize_string().size() == 0)
			iter->set_bodysize();
		if(iter->get_location().size() == 0)
			throw std::invalid_argument("ERROR: missing location !");
		tmp_loc = iter->get_location();
		iter_loc = tmp_loc.begin();
		while(iter_loc != tmp_loc.end())
		{
			if(iter_loc->get_root().size() == 0 || iter_loc->get_method_list().size() == 0)
				throw std::invalid_argument("ERROR: missing some info !");
			iter_loc++;
		}
		iter++;
	}
}

void	ft_add_server(std::fstream &file, Server &server)
{
	std::string		line;
	std::string		befor;
	std::string		after;
	bool			flag;
	Server_obj		config_file;
	int				end_serv = 0;

	while(std::getline(file, line))
	{
		if(line != "server {" && line.size() != 0)
			throw std::invalid_argument("ERROR: Server block missing !");
		else if(line == "server {")
		{
			end_serv = 1;
			break;
		}
		else if(line.size() == 0)
			continue;
	}
	while (std::getline(file, line) && end_serv == 1)
	{
		if(line == "}")
		{
			server.push_back(config_file);
			end_serv = 0;
			break;
		}
		flag = line.find(' ');
		if((flag && line.size() >= 1) || line.size() >= 1)
		{
			ft_split_line(line, befor, after);			
			if(befor == "location")
				ft_add_location(file, config_file, server, befor, after);
			else
				ft_add_block(config_file, befor, after);
		}
	}
	if(end_serv == 1)
		throw std::invalid_argument("ERROR: Unclosed server !");
}

void	ft_split_line(std::string &line, std::string &befor, std::string &after)
{
	size_t	i = 0;

	while(line[i] <= 32 && i < line.size())
		i++;
	line = line.substr(i, line.size());
	i = line.find(' ');
	befor = line.substr(0, i);
	line.erase(0, i);
	after = line.substr(line.find(' ') + 1, line.size());
}