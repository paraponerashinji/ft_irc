#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <cctype>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include <cstdio>

class Client
{
private:
    int                 _fd;            // la socket
    std::string         _nickname;      // pseudo du client
    std::string         _username;      // nom d'utilisateur
    std::string         _realname;      // vrai nom
    std::string         _hostname;      // adresse IP du client
    std::string         _buffer;        // accumule les données reçues
    bool                _is_registered; // true = client totalement identifié (NICK + USER reçus)
    std::vector<std::string> _channels; // liste des channels où il est

public:

    Client();
    Client(int fd);
    ~Client();

    int                 getFd() const;
    std::string         getNickname() const;
    std::string         getUsername() const;
    std::string         getRealname() const;
    std::string         getHostname() const;
    std::string         getBuffer() const;
    bool                isRegistered() const;
    std::vector<std::string>    getChannels() const;

    void                setFd(int fd);
    void                setNickname(std::string nickname);
    void                setUsername(std::string username);
    void                setRealname(std::string realname);
    void                setHostname(std::string hostname);
    void                setBuffer(std::string buffer);
    void                setRegistered(bool is_registered);
    void                addChannel(std::string channel);
    void                removeChannel(std::string channel);

    void                appendToBuffer(std::string data);
    void                clearBuffer();

    void                sendMessage(Client *receiver, std::string msg);
};

#endif

