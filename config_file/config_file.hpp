#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

// typedef std::vector<Server_obj> Server;
// typedef std::vector<location_obj> location;

class location_obj
{
private:
	typedef std::list<std::string>	methods;
	std::string	location;
	std::string	root;
	std::string	index;
	std::string	redir;
	std::string	upload;
	methods		methods_list;
	// bool		cgi;
public:
	void	loc_setter(std::string &str, std::string &attr);
	void	loc_getter();
	void	ft_add_methods_list(methods &methods_list, std::string &after);
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
	void	getter();
	// void	set_body_size();
	// void	set_loc_path();
};

typedef std::vector<Server_obj> Server;

//////////////////////////////
void	ft_read(char *file_name, Server &config_file);
void	ft_add_server(std::fstream &file, Server &config_file);
void	ft_add_block(Server_obj &config_file, std::string &befor, std::string &after);
void	ft_add_location(std::fstream &file, Server_obj &config_file, Server Cont_server, std::string &befor, std::string &after);
void	ft_split_line(std::string &line, std::string &befor, std::string &after);



// void	ft_add_block(Server &config_file, std::string &befor, std::string &after);
// void	ft_check_bracket(std::fstream &file, std::string &line);
// void	ft_add_location(Server &config_file, std::string &befor, std::string &after);



#endif