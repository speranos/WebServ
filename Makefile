NAME = webserv
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
SRCS = config_file/*.cpp *.cpp socket/socket.cpp request/*.cpp

all : $(NAME)
$(NAME): $(SRCS)
	@c++ $(SRCS) $(CFLAGS) -o $(NAME)
	@echo YOU ARE READY TO GO ...!
clean :
	@rm -rf $(NAME)
	@echo CLEAN ...!
fclean : clean
	@echo RESET ...!
re : fclean all