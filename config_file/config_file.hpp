#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

// typedef std::vector<Server_obj> Server;
// typedef std::vector<location_obj> location;

class location_obj
{
private:
	std::string	root;
	std::string	location;
	std::string	index;
	std::string	redir;
	std::string	methods;
	std::string	upload;
	// bool		cgi;
public:
	void	loc_setter(std::string &str, std::string &attr);
};
/////////////////////////////
class Server_obj
{
private:
	std::string		port;
	std::string		host;
	std::string		body_size;
	typedef std::vector<location_obj> location;
	location		C_location;
public:
	void	setter(std::string &str, std::string &attr);
	void	push(location_obj &loc_obj);
	// void	set_body_size();
	// void	set_loc_path();
};

typedef std::vector<Server_obj> Server;

//////////////////////////////
void	ft_read(char *file_name, Server &config_file);
void	ft_add_server(std::fstream &file, Server &config_file);
void	ft_add_block(Server_obj &config_file, std::string &befor, std::string &after);
void	ft_add_location(std::fstream &file, Server_obj &config_file, Server Cont_server, std::string &befor, std::string &after);


// void	ft_add_block(Server &config_file, std::string &befor, std::string &after);
// void	ft_check_bracket(std::fstream &file, std::string &line);
// void	ft_add_location(Server &config_file, std::string &befor, std::string &after);



#endif