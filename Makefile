NAME = webserv
CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g
SRCS = config_file/*.cpp *.cpp socket/socket.cpp request/*.cpp

all : $(NAME)
$(NAME): $(SRCS)
	@c++ $(SRCS) $(CFLAGS) -o $(NAME)
	@echo YOU ARE READY TO GO ...!
clean :
	@rm -rf request_body_*.*
	@rm -rf $(NAME)
	@echo CLEAN ...!
fclean : clean
	@echo RESET ...!
re : fclean all