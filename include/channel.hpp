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
        Channel(Server*, std::string, Client*);
        Channel(Server*, std::string, std::string, Client*);
        ~Channel();
        std::string hash_password(const std::string&);
        void    join(Client*);
        void    join(Client*, std::string);
        void    quit(Client*);
        void    kick(Client *user, Client *target);
        void    editTopic(Client *user, std::string topic);
        void    editInvite_only(Client *user, bool value);
        void    editTopic_admin_only(Client *user, bool value);
        void    set_password(Client *user, std::string password);
        void    add_Admin(Client *user, Client *target);
        void    add_Invited(Client *user, Client *target);
        void    editUser_limit(Client *user, int value);
        void    broadcast(Client *, std::string);
        void    remove_password(Client *user);
        void    remove_Admin(Client *user, Client *target);
        size_t          getUserLimit();
        size_t          getUsersize();
        std::string     getName() const;
        std::vector<Client*>  getClients();
        std::vector<Client*>  getAdmins();
        bool    isAdmin(Client *client);
        Client  *getAdmins(Client*);
        Client  *getClients(Client*);
        Client *getClient(std::string user);
        bool    isTopicAdmin();
        void    sendChanWelcome(Client *user);
};
#endif