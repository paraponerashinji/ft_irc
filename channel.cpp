#include "include/channel.hpp"
#include "include/server.hpp"
#include "include/client.hpp"

Channel::Channel()
{
    _server = NULL;
    _name = "";
    _user_limit = 0;
    _user_size = 0;
    _topic = "";
    _room_password = "";
    _invite_only = false;
    _topic_admin_only = false;
    _room_password_active = false;
}

Channel::Channel(Server *server, std::string name, Client client)
    : _server(server), _name(name)
{
    _Admins.push_back(&client);
    _Clients.push_back(&client);
    _user_limit = 0;
    _user_size = 0;
    _topic = "";
    _room_password = "";
    _invite_only = false;
    _topic_admin_only = false;
    _room_password_active = false;
}

Channel::Channel(Server *server, std::string name, std::string password, Client client)
    : _server(server), _name(name)
{
    _Admins.push_back(&client);
    _Clients.push_back(&client);
    _room_password = password;
    _user_limit = 0;
    _user_size = 0;
    _topic = "";
    _invite_only = false;
    _topic_admin_only = false;
    _room_password_active = true;
}

Channel::~Channel()
{
}

std::string Channel::hash_password(const std::string &password)
{
    return password;
}

std::string Channel::getName() const
{
    return _name;
}