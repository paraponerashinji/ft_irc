#include "server.hpp"
#include "./include/client.hpp"
#include "./include/channel.hpp"

Server::Server() {
    _password = "";
    _serverFd = -1;
}

Server::Server(std::string password, int server_fd) : _password(password), _serverFd(server_fd) {}

Server::~Server() {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() >= 0)
            close(it->getFd());
    }
    _clients.clear();
    _channels.clear();
    if (_serverFd >= 0)
        close(_serverFd);
    _serverFd = -1;
}

int Server::getServerFd() const {
    return _serverFd;
}

Client Server::getClient(int fd) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == fd)
            return *it;
    }
    return Client();
}

Client &Server::getClientRef(int fd) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == fd)
            return *it;
    }
    static Client empty_client(-1);
    return empty_client;
}

Client Server::getClient(std::string nickname) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getNickname() == nickname)
            return *it;
    }
    return Client();
}

Client &Server::getClientRef(std::string nickname) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getNickname() == nickname)
            return *it;
    }
    static Client empty_client(-1);
    return empty_client;
}

Channel Server::getChannel(std::string name) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == name)
            return *it;
    }
    return Channel();
}

std::vector<Channel> Server::getChannels() const {
    return _channels;
}

std::string Server::getPassword() const {
    return _password;
}

void Server::addClient(Client c) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == c.getFd())
            return;
    }
    _clients.push_back(c);
}

void Server::removeClient(int fd) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getFd() == fd) {
            if (it->getFd() >= 0)
                close(it->getFd());
            _clients.erase(it);
            return;
        }
    }
}

void Server::createChannel(std::string name, Client creator) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == name)
            return;
    }

    Channel new_channel(*this, name, creator);
    _channels.push_back(new_channel);
}

void Server::sendMessage(Client &c, std::string message) {
    if (message.empty() || c.getFd() < 0)
        return;

    std::string data = message;
    if (data[data.size() - 1] != '\n')
        data += "\r\n";

    send(c.getFd(), data.c_str(), data.size(), 0);
}

void Server::receiveMessage(Client &c, std::string message) {
    if (message.empty())
        return;

    std::string line = message;
    if (!line.empty() && line[line.size() - 1] == '\n')
        line.erase(line.size() - 1);
    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);

    if (line.empty())
        return;

    std::string command = line;
    std::string param = "";

    std::string::size_type space = line.find(' ');
    if (space != std::string::npos)
    {
        command = line.substr(0, space);
        param = line.substr(space + 1);
    }

    std::string upper = command;
    for (std::string::size_type i = 0; i < upper.size(); ++i)
        upper[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(upper[i])));

    if (upper == "PASS")
    {
        if (param == _password)
            c.setRegistered(true);
        return;
    }

    if (upper == "NICK")
    {
        c.setNickname(param);
        return;
    }

    if (upper == "USER")
    {
        std::string username = param;
        std::string::size_type first_space = username.find(' ');
        if (first_space != std::string::npos)
            username = username.substr(0, first_space);
        c.setUsername(username);
        return;
    }

    if (upper == "JOIN")
    {
        std::string channel_name = param;
        std::string::size_type first_space = channel_name.find(' ');
        if (first_space != std::string::npos)
            channel_name = channel_name.substr(0, first_space);

        if (!channel_name.empty())
        {
            createChannel(channel_name, c);
            c.addChannel(channel_name);
        }
        return;
    }

    if (upper == "QUIT")
    {
        removeClient(c.getFd());
        return;
    }

    if (upper == "PRIVMSG")
    {
        if (param.empty())
            return;

        std::string::size_type first_space = param.find(' ');
        if (first_space == std::string::npos)
            return;

        std::string target = param.substr(0, first_space);
        std::string text = param.substr(first_space + 1);
        if (!text.empty() && text[0] == ':')
            text.erase(0, 1);

        Client target_client = getClient(target);
        if (target_client.getFd() != -1)
            sendMessage(target_client, "PRIVMSG " + target + " :" + text + "\r\n");
        return;
    }
}