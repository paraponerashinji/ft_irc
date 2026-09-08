#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <sstream>
class IrcException: public std::exception
{
    public:
        virtual int errorCode() const = 0;
        std::string target;
        IrcException(const std::string &target);
};

class   ERR_NOSUCHNICK: public IrcException
{
    int errorCode() const;
    ERR_NOSUCHNICK(const std::string &target) : IrcException(target) {}
};

class   ERR_NOSUCHCHANNEL: public IrcException
{
    int errorCode() const;
    ERR_NOSUCHCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_TOOMANYTARGETS: public IrcException
{
    int errorCode() const;
    ERR_TOOMANYTARGETS(const std::string &target) : IrcException(target) {}
};

class   ERR_NOTEXTTOSEND: public IrcException
{
    int errorCode() const;
    ERR_NOTEXTTOSEND(const std::string &target) : IrcException(target) {}
};

class   ERR_UNKNOWNCOMMAND: public IrcException
{
    int errorCode() const;
    ERR_UNKNOWNCOMMAND(const std::string &target) : IrcException(target) {}
};

class   ERR_NONICKNAMEGIVEN: public IrcException
{
    int errorCode() const;
    ERR_NONICKNAMEGIVEN(const std::string &target) : IrcException(target) {}
};

class   ERR_ERRONEUSNICKNAME: public IrcException
{
    int errorCode() const;
    ERR_ERRONEUSNICKNAME(const std::string &target) : IrcException(target) {}
};

class   ERR_NICKNAMEINUSE: public IrcException
{
    int errorCode() const;
    ERR_NICKNAMEINUSE(const std::string &target) : IrcException(target) {}
};

class   ERR_NICKCOLLISION: public IrcException
{
    int errorCode() const;
    ERR_NICKCOLLISION(const std::string &target) : IrcException(target) {}
};

class   ERR_USERNOTINCHANNEL: public IrcException
{
    int errorCode() const;
    ERR_USERNOTINCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_NOTONCHANNEL: public IrcException
{
    int errorCode() const;
    ERR_NOTONCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_USERONCHANNEL: public IrcException
{
    int errorCode() const;
    ERR_USERONCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_NOLOGIN: public IrcException
{
    int errorCode() const;
    ERR_NOLOGIN(const std::string &target) : IrcException(target) {}
};

class   ERR_NOTREGISTERED: public IrcException
{
    int errorCode() const;
    ERR_NOTREGISTERED(const std::string &target) : IrcException(target) {}
};

class   ERR_NEEDMOREPARAMS: public IrcException
{
    int errorCode() const;
    ERR_NEEDMOREPARAMS(const std::string &target) : IrcException(target) {}
};

class   ERR_ALREADYREGISTRED: public IrcException
{
    int errorCode() const;
    ERR_ALREADYREGISTRED(const std::string &target) : IrcException(target) {}
};

class   ERR_PASSWDMISMATCH: public IrcException
{
    int errorCode() const;
    ERR_PASSWDMISMATCH(const std::string &target) : IrcException(target) {}
};

class   ERR_KEYSET: public IrcException
{
    int errorCode() const;
    ERR_KEYSET(const std::string &target) : IrcException(target) {}
};

class   ERR_CHANNELISFULL: public IrcException
{
    int errorCode() const;
    ERR_CHANNELISFULL(const std::string &target) : IrcException(target) {}
};

class   ERR_UNKNOWNMODE: public IrcException
{
    int errorCode() const;
    ERR_UNKNOWNMODE(const std::string &target) : IrcException(target) {}
};

class   ERR_INVITEONLYCHAN: public IrcException
{
    int errorCode() const;
    ERR_INVITEONLYCHAN(const std::string &target) : IrcException(target) {}
};

class   ERR_BADCHANNELKEY: public IrcException
{
    int errorCode() const;
    ERR_BADCHANNELKEY(const std::string &target) : IrcException(target) {}
};

class   ERR_CHANOPRIVSNEEDED: public IrcException
{
    int errorCode() const;
    ERR_CHANOPRIVSNEEDED(const std::string &target) : IrcException(target) {}
};

#endif