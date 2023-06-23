#include "../webserv.hpp"

void	location_obj::loc_setter(std::string &str, std::string &attr)
{
	if(str == "location")
		location = attr;
	else if(str == "root")
		root = attr;
	else if(str == "methods")
		ft_add_methods_list(attr);
	else if(str == "upload")
		upload = attr;
	else if(str == "redir")
		redir = attr;
	else if(str == "index")
		index = attr;
	else if(str == "cgi" || str == "auto_index")
		ft_set_bool(str, attr);
	else
		throw std::invalid_argument("ERROR: Unsupported directive !");
}

void	location_obj::ft_set_bool(std::string &befor, std::string &after)
{
	if(after == "on")
	{
		if(befor == "cgi")
			cgi = true;
		else
			auto_index = true;
	}
	else if(after == "off")
	{
		if(befor == "cgi")
			cgi = false;
		else
			auto_index = false;
	}
	else
		throw std::invalid_argument("ERROR: This directive can only be on or off !");
}

void	location_obj::ft_add_methods_list(std::string &after)
{
	size_t		start = 0;
	size_t		end = 0;
	std::string	push;

	if(after.size())
	{
		while(end < after.size())
		{
			end = after.find(' ', start);
			push = after.substr(start, end);
			if((push != "GET" && push != "POST" && push != "DELETE") && methods_list.size() >= 3)
				throw std::invalid_argument("ERROR: Unsupported method !");
			methods_list.push_back(push);
			after.erase(start, (end + 1));
		}
	}
}

void	location_obj::loc_getter()
{
	std::cout << location << "\n" << root << "\n" << "\n" << index << "\n" << redir << "\n" << upload << "\n" << cgi << "\n" << auto_index << std::endl;
	methods::iterator	iter = methods_list.begin();
	while (iter != methods_list.end())
	{
		std::cout << *iter << std::endl;
		iter++;
	}
	
}

void	Server_obj::getter()
{
	size_t	i = 0;
	std::cout << port << "\n" << host << "\n" << body_size << std::endl;
	while(i < C_location.size())
		C_location[i++].loc_getter();
}

void	Server_obj::setter(std::string &str, std::string &attr)
{
	if(str == "listen")
		port = attr;
	else if(str == "host")
		host = attr;
	else
		throw std::invalid_argument("ERROR: Unsupported directive !");
}

void	Server_obj::push(location_obj &loc_obj)
{
	C_location.push_back(loc_obj);
}

/////////////////////////////////////////////////

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
			{
				ft_add_location(file, config_file, server, befor, after);
			}
			else
				ft_add_block(config_file, befor, after);
		}
		else if(file.peek() == EOF)
			std::cout << "WAAAQAAAAAAA HASSSSSSSAAAAAAAAAAANNNNNNN" << std::endl;
	}
	if(end_serv == 1)
		throw std::invalid_argument("ERROR: Unclosed server !");
}

void	ft_split_line(std::string &line, std::string &befor, std::string &after)
{
	size_t	i;
	i = 0;
	while(line[i] <= 32 && i < line.size())
		i++;
	line = line.substr(i, line.size());
	i = line.find(' ');
	befor = line.substr(0, i);
	line.erase(0, i);
	after = line.substr(line.find(' ') + 1, line.size());
}

void	ft_add_location(std::fstream &file, Server_obj &config_file, Server server, std::string &befor, std::string &after)
{
	std::string		line;
	location_obj	loc_obj;
	bool 			flag;

	flag = after.find('{');
	if(!flag || after.find('{') != (after.size() - 1))
		throw std::invalid_argument("ERROR: Bracket syntax error !");
	after = after.substr(0, after.find(' '));
	loc_obj.loc_setter(befor, after);
	while(std::getline(file, line))
	{
		if(line == "}")
		{
			config_file.push(loc_obj);
			break;
		}
		flag = line.find(' ');
		if((flag && line.size() > 1) || line.size() > 1)
		{
			ft_split_line(line, befor, after);
			loc_obj.loc_setter(befor, after);
		}
		else if(file.peek() == EOF)
			std::cout << "WAAAAAAAAAAA HASSSSSSSAAAAAAAAAAANNNNNNN" << std::endl;
	}
	(void)server;
}

void	ft_add_block(Server_obj &config_file, std::string &befor, std::string &after)
{
	after = after.substr(after.find_first_not_of(' '));
	config_file.setter(befor, after);
}

