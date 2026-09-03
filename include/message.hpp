#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "exception.hpp"
class Server;
class Client;
class Channel;
class Join;

class Message
{
    protected:
        Server      *_server;
        Client      *_user;
        std::string _text;
    public:
        Message(Server*, Client*, std::string);
        virtual ~Message();
        virtual void exec(std::string) = 0;
};

class Pass: public Message
{
    public:
        void exec(std::string);

    class   ClientExistAlreadyException: public IrcException
    {
        int errorCode() const;
    };
};

class Nick: public Message
{
    public:
        void exec(std::string);
};

class User: public Message
{
    public:
        void exec(std::string);
    class   UserNameAlreadyTakenException: public IrcException
    {
        int errorCode() const;
    };
    class   UserNameAlreadySetException: public IrcException
    {
        int errorCode() const;
    };
};

class Privmsg: public Message
{
    public:
        std::vector<std::string> split(std::string);
        void         exec(std::string);
        class   InvalidTargetInputException: public IrcException
        {
            int errorCode() const;
        };
};

class Quit: public Message
{
    public:
        void exec(std::string);
};

class Join: public Message
{
    private:
        std::string _password;
    public:
        std::string getPassword();
        void exec(std::string);
};

class Topic: public Message
{
    public:
        void exec(std::string);
};

class Part: public Message
{
    public:
        void exec(std::string);
};

class Kick: public Message
{
    public:
        void exec(std::string);
};

class Invite: public Message
{
    public:
        void exec(std::string);
};

class Mode: public Message
{
    public:
        void exec(std::string);
};

#endif