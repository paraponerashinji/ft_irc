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
        std::string target;
        IrcException(const std::string &target);
};

class   ERR_NOSUCHNICK: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOSUCHNICK(const std::string &target) : IrcException(target) {}
};

class   ERR_NOSUCHCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOSUCHCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_TOOMANYTARGETS: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_TOOMANYTARGETS(const std::string &target) : IrcException(target) {}
};

class   ERR_NOTEXTTOSEND: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOTEXTTOSEND(const std::string &target) : IrcException(target) {}
};

class   ERR_UNKNOWNCOMMAND: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_UNKNOWNCOMMAND(const std::string &target) : IrcException(target) {}
};

class   ERR_NONICKNAMEGIVEN: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NONICKNAMEGIVEN(const std::string &target) : IrcException(target) {}
};

class   ERR_ERRONEUSNICKNAME: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_ERRONEUSNICKNAME(const std::string &target) : IrcException(target) {}
};

class   ERR_NICKNAMEINUSE: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NICKNAMEINUSE(const std::string &target) : IrcException(target) {}
};

class   ERR_NICKCOLLISION: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NICKCOLLISION(const std::string &target) : IrcException(target) {}
};

class   ERR_USERNOTINCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_USERNOTINCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_NOTONCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOTONCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_USERONCHANNEL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_USERONCHANNEL(const std::string &target) : IrcException(target) {}
};

class   ERR_NOLOGIN: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOLOGIN(const std::string &target) : IrcException(target) {}
};

class   ERR_NOTREGISTERED: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NOTREGISTERED(const std::string &target) : IrcException(target) {}
};

class   ERR_NEEDMOREPARAMS: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_NEEDMOREPARAMS(const std::string &target) : IrcException(target) {}
};

class   ERR_ALREADYREGISTRED: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_ALREADYREGISTRED(const std::string &target) : IrcException(target) {}
};

class   ERR_PASSWDMISMATCH: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_PASSWDMISMATCH(const std::string &target) : IrcException(target) {}
};

class   ERR_KEYSET: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_KEYSET(const std::string &target) : IrcException(target) {}
};

class   ERR_CHANNELISFULL: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_CHANNELISFULL(const std::string &target) : IrcException(target) {}
};

class   ERR_UNKNOWNMODE: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_UNKNOWNMODE(const std::string &target) : IrcException(target) {}
};

class   ERR_INVITEONLYCHAN: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_INVITEONLYCHAN(const std::string &target) : IrcException(target) {}
};

class   ERR_BADCHANNELKEY: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_BADCHANNELKEY(const std::string &target) : IrcException(target) {}
};

class   ERR_CHANOPRIVSNEEDED: public IrcException
{
    public:
        int errorCode() const;
        explicit ERR_CHANOPRIVSNEEDED(const std::string &target) : IrcException(target) {}
};

#endif