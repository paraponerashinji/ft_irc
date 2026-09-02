SRCS = source/channel.cpp source/client.cpp source/message.cpp source/server.cpp server.cpp socket.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = IRC
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all