.PHONY:	all clean fclean re

NAME		= ircserv 

SRCS		= ./../lexer/lexer.cpp	\
		  config_socket.cpp	\
		  manage_socket.cpp	\
		  ircserv.cpp

OBJS		= $(SRCS:.cpp=.o)

CXX		= clang++

CXXFLAGS	= -Wall -Werror -Wextra -std=c++98

RM		= rm -f

all:		$(NAME)

$(NAME):	$(OBJS)
	@ $(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	@ $(RM) $(OBJS)

fclean:		clean
	@ $(RM) $(NAME) ./.nameless_history

re:		fclean all
