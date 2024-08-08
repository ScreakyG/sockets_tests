NAME = irc_test

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -MMD -std=c++98 -g

SRCS = main.cpp
SRCS_OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

all: $(NAME)

$(NAME): $(SRCS_OBJS)
		$(CXX) $(CXXFLAGS) $(SRCS_OBJS) -o $(NAME)

clean:
		rm -rf $(SRCS_OBJS)
		rm -rf $(DEPS)

fclean:	clean
		rm -rf $(NAME)

re: fclean all

-include $(DEPS)
