#include "../include/channel.hpp"

Channel::Channel() {
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

Channel::Channel(Server *server, std::string name, Client client): _name(name), _server(server)
{
    _Admins.push_back(client);
    _Clients.push_back(client);
    _user_limit = 0;
    _user_size = 0;
    _invite_only = false;
    _topic_admin_only = false;
    _room_password_active = false;
};

Channel::Channel(Server *server, std::string name, std::string password, Client client): _name(name), _server(server)
{
    _Admins.push_back(client);
    _Clients.push_back(client);
    _room_password = hash_password(password);
    _user_limit = 0;
    _user_size = 0;
    _invite_only = false;
    _topic_admin_only = false;
    _room_password_active = true;
};

Channel::~Channel()
{
};

std::string Channel::hash_password(const std::string& password)
{
    unsigned int hash = 67676767;
    for (int i = 0; i < password.length(); i++)
    {
        hash = hash * 31 + password[i];
    }
    std::stringstream ss;
    ss << std::hex << hash;
    return (ss.string());
};

void    Channel::join(Client user)
{
    std::vector<Client>::iterator it;
    it = std::find(_Invited.begin(), _Invited.end(), user);
    if (_invite_only && it == _Invited.end())
        throw MissingInviteException();
    if (_room_password_active && it == _Invited.end())
        throw InvalidPasswordException();
    if (_user_limit != 0 && _user_limit >= _user_size)
        throw ChannelFullException();
    _Clients.push_back(user);
    if (it != _Invited.end())
        _Invited.erase(it);
    _user_size++;
};

void    Channel::join(Client user, Join password)
{
    if (_user_limit != 0 && _user_limit >= _user_size)
        throw ChannelFullException();
    std::vector<Client>::iterator it;
    it = std::find(_Invited.begin(), _Invited.end(), user);
    if (_invite_only && it == _Invited.end())
        throw MissingInviteException();
    if (it != _Invited.end())
    {
        _Clients.push_back(user);*
        _Invited.erase(it);
        _user_size++;
        return;
    }
    if (_room_password_active && hash_password(password.getPassword()) != _password)
        throw InvalidPasswordException();
    _Clients.push_back(user);
    _user_size++;
};

void    Channel::quit(Client user)
{
    std::vector<Client>::iterator it;
    it = std::find(_Admins.begin(), _Admins.end(), user)
    if (it != _Admins.end())
        _Admins.erase(it);
    it = std::find(_Clients.begin(), _Clients.end(), user)
    _Clients.erase(it);
    _user_size--;
    if (_user_size == 1)
        _Admins.push_back(_Clients.front());
    if (_user_size == 0)
        ~Channel();
};

void    Channel::editTopic(std::string topic)
{
    _topic = topic;
};

void    Channel::editInvite_only();
{
    _invite_only = !_invite_only;
};

void    Channel::editTopic_admin_only()
{
    _topic_admin_only = !_topic_admin_only;
};

void    Channel::set_password(std::string password)
{
    _room_password = hash_password(password);
};

void    Channel::add_Admin(Client user)
{
    std::vector<Client>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user)
    if (it == _Clients.end())
        throw MemberNotFoundException();
    _Admins.push_back(user);
};

void    Channel::editUser_limit()
{
    _user_limit = !_user_limit;
};

void    Channel::broadcast(Client user, std::string message);
{
    std::vector<Client>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user)
    if (it == _Clients.end())
        throw MemberNotFoundException();
    for (std::vector<Client>::iterator it = _Clients.begin(); it ! _Clients.end(); ++it)
    {
        _server::send_message(*it, message);
    }
};

void    Channel::add_Invited(Client user)
{
    std::vector<Client>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user);
    if (it != _Clients.end())
        throw MemberAlreadyHereException();
    it = std::find(_Invited.begin(), _Invited.end(), user);
    if (it != _Invited.end())
        throw UserAlreadyInvitedException();
    _Invited.push_back(user);
};

Channel::getName() const{
    return _name;
}

vector<Client> Channel::getClients()
{
    return _Clients;
};

vector<Client> Channel::getAdmins()
{
    return _Admins;
};

Client  Channel::getClients(Client user)
{
    std::vector<Client>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user);
    if (it == _Clients.end())
        throw MemberNotFoundException();
    return (user);
};

Client  Channel::getAdmins(Client user)
{
    std::vector<Client>::iterator it;
    it = std::find(_Admins.begin(), _Admins.end(), user)
    if (it == _Admins.end())
        throw UserIsNotAdminException();
    return (user);
};

bool    Channel::isTopicAdmin()
{
    return _topic_admin_only;
};

int Channel::ChannelFullException() const
{
    return (471);
};

int Channel::InvalidPasswordException() const
{
    return (475);
};

int Channel::MissingInviteException() const
{
    return (473);
};

int Channel::MissingAuthorizationException() const
{
    return (481);
};

int Channel::MemberNotFoundException() const
{
    return (401);
};