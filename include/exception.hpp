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
};

class   ERR_NOSUCHNICK: public IrcException
{
    int errorCode() const;
};

class   ERR_NOSUCHCHANNEL: public IrcException
{
    int errorCode() const;
};

class   ERR_TOOMANYTARGETS: public IrcException
{
    int errorCode() const;
};

class   ERR_NOTEXTTOSEND: public IrcException
{
    int errorCode() const;
};

class   ERR_UNKNOWNCOMMAND: public IrcException
{
    int errorCode() const;
};

class   ERR_NONICKNAMEGIVEN: public IrcException
{
    int errorCode() const;
};

class   ERR_ERRONEUSNICKNAME: public IrcException
{
    int errorCode() const;
};

class   ERR_NICKNAMEINUSE: public IrcException
{
    int errorCode() const;
};

class   ERR_NICKCOLLISION: public IrcException
{
    int errorCode() const;
};

class   ERR_USERNOTINCHANNEL: public IrcException
{
    int errorCode() const;
};

class   ERR_NOTONCHANNEL: public IrcException
{
    int errorCode() const;
};

class   ERR_USERONCHANNEL: public IrcException
{
    int errorCode() const;
};

class   ERR_NOLOGIN: public IrcException
{
    int errorCode() const;
};

class   ERR_NOTREGISTERED: public IrcException
{
    int errorCode() const;
};

class   ERR_NEEDMOREPARAMS: public IrcException
{
    int errorCode() const;
};

class   ERR_ALREADYREGISTRED: public IrcException
{
    int errorCode() const;
};

class   ERR_PASSWDMISMATCH: public IrcException
{
    int errorCode() const;
};

class   ERR_KEYSET: public IrcException
{
    int errorCode() const;
};

class   ERR_CHANNELISFULL: public IrcException
{
    int errorCode() const;
};

class   ERR_UNKNOWNMODE: public IrcException
{
    int errorCode() const;
};

class   ERR_INVITEONLYCHAN: public IrcException
{
    int errorCode() const;
};

class   ERR_BADCHANNELKEY: public IrcException
{
    int errorCode() const;
};

class   ERR_CHANOPRIVSNEEDED: public IrcException
{
    int errorCode() const;
};

#endif