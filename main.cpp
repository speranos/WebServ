#include "webserv.hpp"

int main(int ac, char **av)
{
	webserv	config_file;
	if(ac != 2)
	{
		std::cout << "ERROR: More or less then the argument requierd !" << std::endl;
		return(0);
	}
	
	try
	{
		ft_read(av[1], config_file);
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}