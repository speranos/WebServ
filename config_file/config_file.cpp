#include "../webserv.hpp"

void	location_obj::loc_setter(std::string &str, std::string &attr)
{
	if(str == "location")
		location = attr;
	else if(str == "root")
		root = attr;
}

void	Server_obj::setter(std::string &str, std::string &attr)
{
	if(attr == "listen")
		port = str;
	else if(attr == "host")
		host = str;
	// else
	// 	loc_path.loc_setter(str, attr);
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
	size_t			i = 0;
	bool			flag;
	Server_obj		config_file;

	std::getline(file, line);
	if(line != "server {")
		throw std::invalid_argument("ERROR: Server block missing !");
	while (std::getline(file, line))
	{
		// std::cout << line << std::endl;
		flag = line.find(' ');
		if((flag && line.size() >= 1) || line.size() >= 1)
		{
			if(line == "}")
			{
				server.push_back(config_file);
				break;
			}
			i = 0;
			while(line[i] <= 32 && i < line.size())
				i++;
			line = line.substr(i, line.size());

			befor = line.substr(0, line.find(' '));

			after = line.substr(line.find(' ') + 1, line.size());
			
			if(befor == "location")
				ft_add_location(file, config_file, server, befor, after);
			else
				ft_add_block(config_file, befor, after);
		}
		else if(file.peek() == EOF)
			std::cout << "WAAAQAAAAAAA HASSSSSSSAAAAAAAAAAANNNNNNN" << std::endl;
		// else if(line.size() >= 1)
		// 	ft_check_bracket(file, line);
	}
}

void	ft_add_location(std::fstream &file, Server_obj &config_file, Server server, std::string &befor, std::string &after)
{
	std::string		line;
	location_obj	loc_obj;

	bool flag = after.find('{');
	if(!flag || after.find('{') != (after.size() - 1))
		throw std::invalid_argument("ERROR: Bracket syntax error !");
	after = after.substr(0, after.find(' '));
	config_file.setter(befor, after);
	size_t	i = 0;
	while(std::getline(file, line))
	{
		if(line == "}")
		{
			config_file.push(loc_obj);
			break;
		}
		bool flag = line.find(' ');
		if((flag && line.size() > 1) || line.size() > 1)
		{
			i = 0;
			while(line[i] <= 32 && i < line.size())
				i++;
			line = line.substr(i, line.size());

			befor = line.substr(0, line.find(' '));

			after = line.substr(line.find(' ') + 1, line.size());
			loc_obj.loc_setter(befor, after);
		}
		else if(file.peek() == EOF)
			std::cout << "WAAAQAAAAAAA HASSSSSSSAAAAAAAAAAANNNNNNN" << std::endl;
	}
	(void)server;
}

void	ft_add_block(Server_obj &config_file, std::string &befor, std::string &after)
{
	// std::cout << "befor ========= '" << befor << "'" << std::endl;
	after = after.substr(after.find_first_not_of(' '));
	// std::cout << "after ========= '" << after << "'\n" << std::endl;
	config_file.setter(befor, after);
}