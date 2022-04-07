.PHONY:	all clean fclean re

NAME		= server

SRCS		= server.cpp		\
		  ./../lexer/lexer.cpp

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
	@ $(RM) $(NAME)

re:		fclean all
