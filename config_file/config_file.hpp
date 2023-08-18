#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

// typedef std::vector<Server_obj> Server;
// typedef std::vector<location_obj> location;

typedef std::list<std::string>	methods;
class location_obj
{
private:
	std::string	location;
	std::string	root;
	std::string	index;
	std::string	redir;
	std::string	upload;
	methods		methods_list;
	bool		auto_index;
	bool		cgi;
public:
	bool	get_auto_index()const
	{
		return (auto_index);
	};
	bool	get_cgi()const
	{
		return (cgi);
	};
	std::string	get_index()const
	{
		return (index);
	};
	std::string get_redir()const{
		return this->redir;
	};
	void	loc_setter(std::string &str, std::string &attr);
	void	loc_getter();
	std::string	get_location()const;
	std::string	get_upload()const;
	std::string	get_root()const;
	methods get_method_list();
	void	ft_add_methods_list(std::string &after);
	void	ft_set_bool(std::string &befor, std::string &after);
};
/////////////////////////////
class Server_obj
{
private:
	typedef std::vector<location_obj>	location;
	typedef std::map<std::string ,std::string>	error_page;
	std::string		port;
	std::string		host;
	int				body_size;
	location		C_location;
	error_page		error;


public:
	void	setter(std::string &str, std::string &attr);
	void	push(location_obj &loc_obj);
	void	ft_add_error_page(std::string &str);
	void	getter();
	std::string	get_port();
	std::string	get_host();
	unsigned long	getBodySize();
	std::vector<location_obj> 	get_location();
	void		set_adrr(sockaddr_in &address);
};

typedef std::vector<Server_obj> Server;

//////////////////////////////
void	ft_read(char *file_name, Server &config_file);
void	ft_add_server(std::fstream &file, Server &config_file);
void	ft_add_block(Server_obj &config_file, std::string &befor, std::string &after);
void	ft_add_location(std::fstream &file, Server_obj &config_file, Server Cont_server, std::string &befor, std::string &after);
void	ft_split_line(std::string &line, std::string &befor, std::string &after);
void	final_check(Server &config_file);



// void	ft_add_block(Server &config_file, std::string &befor, std::string &after);
// void	ft_check_bracket(std::fstream &file, std::string &line);
// void	ft_add_location(Server &config_file, std::string &befor, std::string &after);



#endif