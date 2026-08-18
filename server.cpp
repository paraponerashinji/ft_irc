#include "server.hpp"

Server::Server() {
    _password = "";
    _serverFd = -1;
}

Server::Server(std::string password, int server_fd) : _password(password), _serverFd(server_fd) {}

Server::~Server() {
    for (int i = 0; i < _clients.size(); ++i) {
        _clients[i] = NULL;
    }
    for (int i = 0; i < _channels.size(); ++i) {
        _channels[i] = NULL;
    }

    _serverFd = -1;
}

int Server::getServerFd() const { return _serverFd }

Client  Server::getClient(int fd) {
    for (int i = 0; i < _clients.size(); ++i) {
        if (_clients[i].getFd() == fd) {
            return _clients[i];
        }
    }
}

Client  Server::getClient(std::string nickname) {
    for (int i = 0; i < _clients.size(); ++i) {
        if (_clients[i].getNickname() == nickname) {
            return _clients[i];
        }
    }
}

Channel     Server::getChannel(std::string name) {
    for (int i = 0; i < _channels.size(); ++i) {
        if (_channels[i]._name = name) {
            return _channel[i];
        }
    }
}

std::vector<Channel>    Server::getChannels() const {
    return _channels;
}


std::string     Server::getPassword() const {
    return _password;
}

void    Server::addClient(Client c) {
    for (int i = 0; i < _clients.size(); ++i) {
        if (_clients[i].getFd() == c.getFd()) {
            return;
        }
    }
    _clients[i] = c;
    return ;
}

void    Server::removeClient(int fd) {
    for (int i = 0; i < _clients.size(); ++i) {
        if (_clients[i].getFd() == fd) {
            _clients[i].erase();
            return;
        }
    }
    return ;
}

void    Server::createChannel(std::string name, Client creator) {

}

void    Server::sendMessage(Client c, std::string message) {

}

void    Server::receiveMessage(Client c, std::string message) {

}