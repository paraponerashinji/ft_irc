#include "server.hpp"

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

Client Server::getClient(std::string nickname) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getNickname() == nickname)
            return *it;
    }
    return Client();
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

    c.appendToBuffer(message);

    std::string buffer = c.getBuffer();
    while (true) {
        std::string::size_type pos = buffer.find("\r\n");
        if (pos == std::string::npos)
            break;

        std::string line = buffer.substr(0, pos);
        buffer.erase(0, pos + 2);

        if (line.empty())
            continue;

        std::string command = line;
        if (!command.empty() && command[command.size() - 1] == '\n')
            command.erase(command.size() - 1);
        if (!command.empty() && command[command.size() - 1] == '\r')
            command.erase(command.size() - 1);

        std::string upper = command;
        for (std::string::size_type i = 0; i < upper.size(); ++i)
            upper[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(upper[i])));

        std::string param = "";
        std::string::size_type space = command.find(' ');
        if (space != std::string::npos) {
            param = command.substr(space + 1);
            command = command.substr(0, space);
        }

        if (upper == "PASS") {
            if (param == _password)
                c.setRegistered(true);
            continue;
        }

        if (upper == "NICK") {
            c.setNickname(param);
            continue;
        }

        if (upper == "USER") {
            c.setUsername(param);
            continue;
        }

        if (upper == "JOIN") {
            createChannel(param, c);
            continue;
        }

        if (upper == "QUIT") {
            removeClient(c.getFd());
            return;
        }

        if (upper == "PRIVMSG") {
            if (param.empty())
                continue;
            std::string::size_type pos2 = param.find(' ');
            if (pos2 != std::string::npos) {
                std::string target = param.substr(0, pos2);
                std::string text = param.substr(pos2 + 1);
                if (!text.empty() && text[0] == ':')
                    text.erase(0, 1);
                for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
                    if (it->getNickname() == target)
                        sendMessage(*it, "PRIVMSG " + target + " :" + text + "\r\n");
                }
            }
            continue;
        }
    }

    c.setBuffer(buffer);
}