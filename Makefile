.PHONY:	all test clean fclean re parser

NAME		= 	server.exe

CXX			= 	clang++

FOLDERS     = ./parser/ ./server/ ./test/ ./command/ 

INCLUDE_HEADERS = $(addprefix -I, $(FOLDERS))

CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98 $(INCLUDE_HEADERS)

PARSER_SRCS	=	$(addprefix ./parser/, lexer.cpp parser.cpp)

SERVER_SRCS	=	$(addprefix ./server/, config_socket.cpp manage_socket.cpp)

CMD_SRCS	=	$(addprefix ./command/, Command.cpp Nick.cpp User.cpp)

SRCS		= 	$(PARSER_SRCS) $(SERVER_SRCS) $(CMD_SRCS) main.cpp

OBJS		= 	$(SRCS:.cpp=.o)

RM			= 	rm -f

# TESTS


ifeq (test, $(firstword $(MAKECMDGOALS)))
	CXXFLAGS	+= 	-g -fsanitize=address 
	ifeq ($(word 2, $(MAKECMDGOALS)), parser)
		TEST_SRCS	= $(PARSER_SRCS) $(CMD_SRCS) ./test/test_parser.cpp
		TEST		= parser.debug
	else
		TEST_SRCS	= $(PARSER_SRCS) $(SERVER_SRCS) ./test/test_irc.cpp	
		TEST		= irc.debug
	endif
endif
ifeq ($(TEST_SRCS),)
	TEST_SRCS = ./test/test_parser.cpp ./test/test_irc.cpp
endif
TEST_OBJS	= $(TEST_SRCS:.cpp=.o)
#-----------------------------------------------------------------------------#

all:	$(NAME)

$(NAME):	$(OBJS)
	@ $(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

test:	$(TEST_OBJS)
	@ $(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST)
clean:
	@ $(RM) $(OBJS) $(TEST_OBJS)

fclean:		clean
	@ $(RM) $(NAME)
	@ $(RM) irc_test parser_test

re:		fclean all

parser: parser.debug
	@ echo "Parser and lexer tests' executable built.."