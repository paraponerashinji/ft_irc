#ifndef SERVER_HPP
#define SERVER_HPP

class Client;
class Channel;
class Server;

int create_server_socket(int port);

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

enum 
{
    NICKNAME,
    USERNAME,
}
Server::create_client(std::string)

Server::create_client(Client, Arg.USERNAME, std::string)
receive_message(Client, std::string);
PRIVMSG Channel
INVITE

send_message(Client, std::string)*/

#include <cctype>
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

class   Server {

    private:

    std::vector<Client>     _clients;
    std::vector<Channel>    _channels;
    std::string             _password;
    int                     _serverFd;

    public:

    Server();
    Server(std::string password, int fd);
    ~Server();

    int     getServerFd() const;

    Client  getClient(int fd);
    Client  getClient(std::string nickname);
    Client  *getClientPtr(std::string nickname);
    Client &getClientRef(int fd);
    Client &getClientRef(std::string nickname);

    Channel *getChannel(std::string name);
    std::vector<Channel>    getChannels() const;
    std::vector<Client>     *getClients() const;
    std::string             getPassword() const;

    void    addClient(Client c);
    void    removeClient(int fd);

    void    createChannel(std::string name, Client creator);

    void    sendMessage(Client &c, std::string message);
    void    receiveMessage(Client &c, std::string message);
    void    ircERROR(Client *user, int code);
};

void    petit_test(int port);

#endif