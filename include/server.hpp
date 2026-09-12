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

#define RESET "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BBLACK   "\033[90m"
#define BRED     "\033[91m"
#define BGREEN   "\033[92m"
#define BYELLOW  "\033[93m"
#define BBLUE    "\033[94m"
#define BMAGENTA "\033[95m"
#define BCYAN    "\033[96m"
#define BWHITE   "\033[97m"

class   Server {

    private:

    std::vector<Client*>     _clients;
    std::vector<Channel*>    _channels;
    std::string             _password;
    int                     _serverFd;
    int                     _port;
    
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
    Client *getClientPtr(int nickname);
    Client &getClientRef(int fd);
    Client &getClientRef(std::string nickname);

    Channel *getChannel(std::string name);
    std::vector<Channel*>    getChannels() const;
    std::vector<Client*>    *getClients() const;

    std::string             getPassword() const;

    void    addClient(Client *c);
    void    removeClient(int fd);

    void    createChannel(std::string name, Client *creator);
    void    createChannel(std::string name, Client *creator, std::string password);
    void    removeChannel(Channel *channel);
    void    sendMessage(Client &c, std::string message);
    void    receiveMessage(Client &c, std::string message);
    bool    isFullyRegistered(Client *sender);
    // HANDLERS
    std::vector<std::string> Parse_Line(std::string text);
    void    Pass(Client* client, const std::vector<std::string>& params);
    void    Nick(Client* client, const std::vector<std::string>& params);
    void    User(Client* client, const std::vector<std::string>& params);
    void    Quit(Client* client, const std::vector<std::string>& params);
    void    join(Client *sender, const std::vector<std::string>& params);
    void    part(Client *sender, const std::vector<std::string>& params);
    void    privmsg(Client *sender, const std::vector<std::string>& params);
    void    kick(Client *sender, const std::vector<std::string>& params);
    void    invite(Client *sender, const std::vector<std::string>& params);
    void    topic(Client *sender, const std::vector<std::string>& params);
    void    mode(Client *sender, const std::vector<std::string>& params);
    bool    isValidNickname(const std::string& nick);
    void    broadcastToCommonChannels(Client* client, const std::string& message);
    std::vector<std::string> parseCommand(std::string line);
    void    executeCommand(Client* client, const std::string& rawLine);
    void    setPort(int port);
    void    sendWelcome(Client *client);
    void    run_server_loop();
    void    run_server_loop(Server &server);
    void    cleanDisconnectedClients();

    void    sendError(Client *user, std::string text);
};

void    petit_test(int port);

#endif