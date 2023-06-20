#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP


class location
{
private:
	std::string	root;
	std::string	methods;
	std::string	index;
	std::string	redir;
	std::string	path;
public:
	void	loc_setter(std::string &str, std::string &attr);
};
/////////////////////////////
class webserv
{
private:
	std::string	port;
	std::string	host;
	std::string	body_size;
	location	loc_path;
public:
	void	setter(std::string &str, std::string &attr);
	// void	set_body_size();
	// void	set_loc_path();

};
//////////////////////////////
void	ft_read(char *file_name, webserv &config_file);
void	ft_add_block(webserv &config_file, std::string &befor, std::string &after);
// void	ft_add_location(webserv &config_file, std::string &befor, std::string &after);



#endif