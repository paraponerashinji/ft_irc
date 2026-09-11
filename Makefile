SRCS = socket.cpp \
	   source/server.cpp \
	   source/handler_chan.cpp \
	   source/handler_serv.cpp \
	   source/channel.cpp \
	   source/client.cpp \
	   source/exception.cpp \
	   main.cpp \
	   irc_loop.cpp 

OBJS = $(SRCS:.cpp=.o)
NAME = IRC
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all