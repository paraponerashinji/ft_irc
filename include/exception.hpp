#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <sstream>
class IrcException: public std::exception
{
    public:
        virtual ~IrcException() throw() {};
        virtual int errorCode() const = 0;
        virtual const char* what() const throw();
        std::string target;
        IrcException(Client *user, const std::string &target);
    protected:
        std::string message;
};

class   ERR_NOSUCHNICK: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOSUCHNICK(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NOSUCHCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOSUCHCHANNEL(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_TOOMANYTARGETS: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_TOOMANYTARGETS(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NOTEXTTOSEND: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOTEXTTOSEND(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_UNKNOWNCOMMAND: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_UNKNOWNCOMMAND(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NONICKNAMEGIVEN: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NONICKNAMEGIVEN(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_ERRONEUSNICKNAME: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_ERRONEUSNICKNAME(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NICKNAMEINUSE: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NICKNAMEINUSE(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NICKCOLLISION: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NICKCOLLISION(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_USERNOTINCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_USERNOTINCHANNEL(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NOTONCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOTONCHANNEL(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_USERONCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_USERONCHANNEL(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NOLOGIN: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOLOGIN(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NOTREGISTERED: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOTREGISTERED(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_NEEDMOREPARAMS: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NEEDMOREPARAMS(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_ALREADYREGISTRED: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_ALREADYREGISTRED(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_PASSWDMISMATCH: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_PASSWDMISMATCH(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_KEYSET: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_KEYSET(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_CHANNELISFULL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_CHANNELISFULL(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_UNKNOWNMODE: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_UNKNOWNMODE(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_INVITEONLYCHAN: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_INVITEONLYCHAN(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_BADCHANNELKEY: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_BADCHANNELKEY(Client *user, const std::string &target) : IrcException(target) {}
};

class   ERR_CHANOPRIVSNEEDED: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_CHANOPRIVSNEEDED(Client *user, const std::string &target) : IrcException(target) {}
};

#endif