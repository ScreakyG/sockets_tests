NAME = irc_test
NAME_CLIENT = client

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -MMD -std=c++98 -g

SRCS = main.cpp
SRCS_OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

CLIENT_SRCS = client.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
DEPS_CLIENT = $(CLIENT_SRCS:.cpp=.d)

all: $(NAME) $(NAME_CLIENT)

$(NAME): $(SRCS_OBJS)
		$(CXX) $(CXXFLAGS) $(SRCS_OBJS) -o $(NAME)

$(NAME_CLIENT): $(CLIENT_OBJS)
		$(CXX) $(CXXFLAGS) $(CLIENT_OBJS) -o $(NAME_CLIENT)

clean:
		rm -rf $(SRCS_OBJS) $(CLIENT_OBJS)
		rm -rf $(DEPS) $(DEPS_CLIENT)

fclean:	clean
		rm -rf $(NAME) $(NAME_CLIENT)

re: fclean all

-include $(DEPS)
-include $(DEPS_CLIENT)
