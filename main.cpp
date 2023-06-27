#include "webserv.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void sendResponse(int clientSocket, const std::string& content, const std::string& contentType) {
	// std::cout << "\n\n" << content << "\n\n" << std::endl;
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " + contentType + "\r\n"
        "Content-Length: " + std::to_string(content.length()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + content;

    send(clientSocket, response.c_str(), response.length(), 0);
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

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
		// getaddrinfo();
		//create multi socket
		//create a structure to handle reading for multiple fd
		//handle non-blocking socket
		//check for host and port if entred twice
		size_t	i = 0;
		while (i < server.size())
		{
			std::cout << "\n\nserver num :: " << i << "\n" << std::endl;
			server[i++].getter();
		}
		int	sck_fd;
		long	valread;
		sockaddr_in	address;
		
		address.sin_port = htons(8020);
		address.sin_addr.s_addr = inet_addr("127.0.0.1");
		sck_fd = socket(AF_INET, SOCK_STREAM, 0);
		int opt = 1;
		if (setsockopt(sck_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			std::cout << "setsockopt" << std::endl;
			exit(0);
		}
		if( (bind(sck_fd, (sockaddr*)&address, sizeof(address))) < 0)
		{
			std::cout << "Bind Failed" << std::endl;
			exit(0);
		}
		if(listen(sck_fd, 5) < 0)
		{
			std::cout << "listen Failed" << std::endl;
			exit(0);	
		}
	int	acc_socket;
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((acc_socket = accept(sck_fd, (struct sockaddr *)&address, (socklen_t*)&address.sin_len ))<0)
        {
			std::cout << "Accept failed" << std::endl;
            exit(0);
        }
        char buffer[30000] = {0};
        valread = read( acc_socket , buffer, 30000);
        printf("%s\n",buffer ); //BUFFER IS THE REQUEST TO PARSS A KHAY SBA333333
		std::string	request = buffer;
		if (request.find(".css") != std::string::npos)
        	sendResponse(acc_socket, readFile("responsive-css-grid-nike-layout/dist/style.css"), "text/css");
		else
        	sendResponse(acc_socket, readFile("responsive-css-grid-nike-layout/dist/index.html"), "text/html");
        printf("------------------Hello message sent-------------------");
        close(acc_socket);
    }
	(void)valread;
}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}