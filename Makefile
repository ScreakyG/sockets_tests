NAME = irc_test
NAME_CLIENT = client
NAME_SERVER = server

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -MMD -std=c++98 -g

SRCS = main.cpp
SRCS_OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

CLIENT_SRCS = client.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
DEPS_CLIENT = $(CLIENT_SRCS:.cpp=.d)

SERVER_SRCS = server.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
DEPS_SERVER = $(SERVER_SRCS:.cpp=.d)

all: $(NAME) $(NAME_CLIENT) $(NAME_SERVER)

$(NAME): $(SRCS_OBJS)
		$(CXX) $(CXXFLAGS) $(SRCS_OBJS) -o $(NAME)

$(NAME_CLIENT): $(CLIENT_OBJS)
		$(CXX) $(CXXFLAGS) $(CLIENT_OBJS) -o $(NAME_CLIENT)

$(NAME_SERVER): $(SERVER_OBJS)
		$(CXX) $(CXXFLAGS) $(SERVER_OBJS) -o $(NAME_SERVER)

clean:
		rm -rf $(SRCS_OBJS) $(CLIENT_OBJS) $(SERVER_OBJS)
		rm -rf $(DEPS) $(DEPS_CLIENT) $(DEPS_SERVER)

fclean:	clean
		rm -rf $(NAME) $(NAME_CLIENT) $(NAME_SERVER)

re: fclean all

-include $(DEPS)
-include $(DEPS_CLIENT)
