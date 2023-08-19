#include "../webserv.hpp"

void	Server_obj::getter()
{
	size_t	i = 0;
	std::cout << port << "\n" << host << "\n" << body_size << std::endl;
	while(i < C_location.size())
		C_location[i++].loc_getter();
	i = 0;
	error_page::iterator iter = error.begin();
	std::pair<std::string, std::string>	p;

	while(iter != error.end())
	{
		p = *iter;
		std::cout << "code >>> " << p.first << " patht to err_page >>> " << p.second << std::endl;
		iter++;
	}
}

std::string	Server_obj::get_port()
{
	return(port);
}

std::string	Server_obj::get_host()
{
	return(host);
}

std::vector<location_obj>	Server_obj::get_location()
{
	return(C_location);
}
// void	Server_obj::set_addr(sockaddr_in &address)
// {
// 	this->address = address;
// }
unsigned long	Server_obj::getBodySize(){
	unsigned long ret = atoi(body_size.c_str());
	return(ret); 
}

std::string Server_obj::getBodySize_string(){
	return(body_size);
}

void	Server_obj::set_bodysize(){
	body_size = "-1";
}

void	Server_obj::setter(std::string &str, std::string &attr)
{
	if(str == "listen")
		port = attr;
	else if(str == "host")
	{
		if(attr == "localhost")
			host = "127.0.0.1";
		else
			host = attr;
	}
	else if(str == "body_size")
	{
		if(attr.size() == 0)
			body_size = "-1";
		else
			body_size = attr;
	}
	else if(str == "error_page")
		ft_add_error_page(attr);
	else
		throw std::invalid_argument("ERROR: Unsupported directive !");
}

void	Server_obj::ft_add_error_page(std::string &str)
{
	std::string	befor;
	std::string	after;

	ft_split_line(str, befor, after);
	if(befor.size() != 3)
		throw std::invalid_argument("ERROR: Unsupported error code !");
	std::pair<std::string, std::string>	p;
	p.first = befor;
	p.second = after;
	error.insert(p);
}

void	Server_obj::push(location_obj &loc_obj)
{
	C_location.push_back(loc_obj);
}

/////////////////////////////////////////////////

void	ft_add_block(Server_obj &config_file, std::string &befor, std::string &after)
{
	after = after.substr(after.find_first_not_of(' '));
	config_file.setter(befor, after);
}