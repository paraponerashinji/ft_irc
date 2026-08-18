#include "../include/client.hpp"
#include <iostream>

Client::Client() : _fd(-1), _nickname(""), _username(""), 
                   _realname(""), _hostname(""), _buffer(""), 
                   _is_registered(false) {}

Client::Client(int fd) : _fd(fd), _nickname(""), _username(""), 
                         _realname(""), _hostname(""), _buffer(""), 
                         _is_registered(false) {}

Client::~Client() {}


int Client::getFd() const { return _fd; }

std::string Client::getNickname() const { return _nickname; }

std::string Client::getUsername() const { return _username; }

std::string Client::getRealname() const { return _realname; }

std::string Client::getHostname() const { return _hostname; }

std::string Client::getBuffer() const { return _buffer; }

bool Client::isRegistered() const { return _is_registered; }

std::vector<std::string> Client::getChannels() const { return _channels; }


void Client::setFd(int fd) { _fd = fd; }

void Client::setNickname(std::string nickname) { _nickname = nickname; }

void Client::setUsername(std::string username) { _username = username; }

void Client::setRealname(std::string realname) { _realname = realname; }

void Client::setHostname(std::string hostname) { _hostname = hostname; }

void Client::setBuffer(std::string buffer) { _buffer = buffer; }

void Client::setRegistered(bool is_registered) { _is_registered = is_registered; }

void Client::addChannel(std::string channel)
{
    _channels.push_back(channel);
}

void Client::removeChannel(std::string channel)
{
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (_channels[i] == channel) {
            _channels.erase(_channels.begin() + i);
        }
    }
}

// ============ UTILITAIRES ============

void Client::appendToBuffer(std::string data)
{
    _buffer += data;
}

void Client::clearBuffer()
{
    _buffer = "";
}
