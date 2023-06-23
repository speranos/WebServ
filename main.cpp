#include "webserv.hpp"

int main(int ac, char **av)
{
	Server	server;
	if(ac != 2)
	{
		std::cout << "ERROR: More or less then the argument requierd !" << std::endl;
		return(0);
	}
	
	try
	{
		ft_read(av[1], server);
		size_t	i = 0;
		while (i < server.size())
		{
			std::cout << "\n\nserver num :: " << i << "\n" << std::endl;
			server[i++].getter();
		}
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}


// TOOOO DOOOOOOOOOOO:
// new line befor the new server
// complete the srttere wiht the attribute 
// check if the server vector are full wiht multiple server
// optimise the code
// do more test
// check for the mandatory attribute are full
// if there is non valid attri in location or server throw
// WAAAAAAA ZBIIIIIIIIIIIIIIII