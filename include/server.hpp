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

    std::vector<Client*>     _clients;
    std::vector<Channel*>    _channels;
    std::string             _password;
    int                     _serverFd;
    
    typedef void (Server::*CommandHandler)(Client* client, const std::vector<std::string>& params);
    std::map<std::string, CommandHandler> _commandMap;

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
    std::vector<Channel*>    getChannels() const;
    std::vector<Client*>    *getClients() const;

    std::string             getPassword() const;

    void    addClient(Client *c);
    void    removeClient(int fd);

    void    createChannel(std::string name, Client *creator);
    void    createChannel(std::string name, std::string password, Client *creator);
    void    sendMessage(Client &c, std::string message);
    void    receiveMessage(Client &c, std::string message);
    void    ircERROR(Client *user, int code);
    bool    isFullyRegistered(Client *sender);
    // HANDLERS
    std::vector<std::string> Parse_Line(std::string text);
    void    Pass(Client* client, const std::vector<std::string>& params);
    void    Nick(Client* client, const std::vector<std::string>& params);
    void    User(Client* client, const std::vector<std::string>& params);
    void    Quit(Client* client, const std::vector<std::string>& params);
    void    join(Client *sender, std::string text);
    void    part(Client *sender, std::string text);
    void    privmsg(Client *sender, std::string text);
    void    kick(Client *sender, std::string text);
    void    invite(Client *sender, std::string text);
    void    topic(Client *sender, std::string text);
    void    mode(Client *sender, std::string text);

    std::vector<std::string> parseCommand(std::string line);
    void    executeCommand(Client* client, const std::string& rawLine);
    void    run_server_loop(Server &server);
    void    cleanDisconnectedClients();

    void    send_Error(Client *user, std::string text);
};

void    petit_test(int port);

#endif