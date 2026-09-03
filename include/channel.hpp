#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include "exception.hpp"
class Server;
class Client;
class Join;
class Message;

class Channel
{
    private:
        Server         *_server;
        std::string    _name;
        std::vector<Client*> _Clients;
        std::vector<Client*> _Admins;
        std::vector<Client*> _Invited;
        size_t         _user_limit;
        size_t         _user_size;
        std::string    _topic;
        std::string    _room_password;
        bool           _invite_only;
        bool           _topic_admin_only;
        bool           _room_password_active;
    public:
        Channel();
        Channel(Server*, std::string, Client);
        Channel(Server*, std::string, std::string, Client);
        ~Channel();
        std::string hash_password(const std::string&);
        void    join(Client*);
        void    join(Client*, std::string);
        void    quit(Client*);
        void    kick(Client*, std::string);
        void    editTopic(std::string);
        void    editInvite_only();
        void    editTopic_admin_only();
        void    set_password(std::string);
        void    add_Admin(Client*);
        void    add_Invited(Client*);
        void    editUser_limit();
        void    broadcast(Client *, std::string);
        std::string     getName() const;
        std::vector<Client*>  getClients();
        std::vector<Client*>  getAdmins();
        Client  *getAdmins(Client*);
        Client  *getClients(Client*);
        bool    isTopicAdmin();
        class   ChannelFullException: public IrcException
        {
            int errorCode() const;
        };
        class   InvalidPasswordException: public IrcException
        {
            int errorCode() const;
        };
        class   MissingInviteException: public IrcException
        {
            int errorCode() const;
        };
        class   MissingAuthorizationException: public IrcException
        {
            int errorCode() const;
        };
        class   MemberNotFoundException: public IrcException
        {
            int errorCode() const;
        };
        class   MemberAlreadyHereException: public IrcException
        {
            int errorCode() const;
        };
        class   UserAlreadyInvitedException: public IrcException
        {
            int errorCode() const;
        };
        class   UserIsNotAdminException: public IrcException
        {
            int errorCode() const;
        };
        class   UserNotInChannelException: public IrcException
        {
            int errorCode() const;
        };
};
#endif