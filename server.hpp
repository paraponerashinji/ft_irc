/*Class   Server {
    private:
    vector<Client> Clients
    vector<Channel> Channels
    std::string password;
    int port;
}

class   Client {
    nickname
    socket
    <string>Channel
}

class Channel {
    vector<Client>
}

receive_message(Client, std::string);
PRIVMSG Channel
INVITE

send_message(Client, std::string)*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <csignal>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

void    petit_test(int port);