#include "../webserv.hpp"

void	location_obj::loc_setter(std::string &str, std::string &attr)
{
	if(str == "location")
	{
		location = attr;
		// std::cout << "location >>> " << location << std::endl;
	}
	else if(str == "root")
	{
		root = attr;
		// std::cout << "root >>> " << root << std::endl;

	}
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

std::string	location_obj::get_location() const
{
	return (this->location);
}

methods location_obj::get_method_list()
{
	return(methods_list);
}

std::string	location_obj::get_root() const
{
	return (this->root);
}

std::string	location_obj::get_upload() const
{
	return (this->upload);
}



///////////////////////////////////////////////////////

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
	}
	(void)server;
}