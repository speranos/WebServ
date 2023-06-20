#include "../webserv.hpp"

void	location::loc_setter(std::string &str, std::string &attr)
{
	if(attr == "location")
		path = attr;
	else if(attr == "root" && path.size() != 0)
		root = attr;
	(void)str;
}

void	webserv::setter(std::string &str, std::string &attr)
{
	if(attr == "listen")
		port = str;
	else if(attr == "host")
		host = str;
	else
		loc_path.loc_setter(str, attr);
}

void	ft_read(char *file_name, webserv &config_file)
{
	std::fstream	file;
	std::string		line;
	std::string		befor;
	std::string		after;
	size_t			i = 0;


	file.open(file_name);
	if(file.fail() || file.eof())
		throw std::invalid_argument("ERROR: File not found !");
	std::getline(file, line);
	if(line != "server {")
		throw std::invalid_argument("ERROR: Server block missing !");
	while (std::getline(file, line))
	{
		if(line.size() > 1)
		{
			i = 0;
			while(line[i] <= 32 && i < line.size())
				i++;
			line = line.substr(i, line.size());
			
			befor = line.substr(0, line.find(' '));

			after = line.substr(line.find(' ') + 1, line.size());
			ft_add_block(config_file, befor, after);
		}
	}
}

void	ft_add_block(webserv &config_file, std::string &befor, std::string &after)
{
	(void)config_file;
	(void)befor;
	std::cout << "befor ========= '" << befor << "'" << std::endl;
	after = after.substr(after.find_first_not_of(' '));
	std::cout << "after ========= '" << after << "'" << std::endl;
}
