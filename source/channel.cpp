#include "../include/channel.hpp"
#include "../include/client.hpp"
#include "../include/server.hpp"
#include "../include/message.hpp"

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

Channel::Channel(Server *server, std::string name, Client client): _server(server), _name(name)
{
    _Admins.push_back(&client);
    _Clients.push_back(&client);
    _user_limit = 0;
    _user_size = 0;
    _invite_only = false;
    _topic_admin_only = false;
    _room_password_active = false;
};

Channel::Channel(Server *server, std::string name, std::string password, Client client): _server(server), _name(name)
{
    _Admins.push_back(&client);
    _Clients.push_back(&client);
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
    for (size_t i = 0; i < password.length(); i++)
    {
        hash = hash * 31 + password[i];
    }
    std::stringstream ss;
    ss << std::hex << hash;
    return (ss.str());
};

void    Channel::join(Client *user)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) != _Clients.end())
    {
        MemberAlreadyHereException();
    }
    if (_invite_only)
    {
        if (std::find(_Invited.begin(), _Invited.end(), user) == _Invited.end())
            throw MissingInviteException();
    }

    if (_user_limit != 0 && _user_size >= _user_limit)
        throw ChannelFullException();
    std::vector<Client*>::iterator it = std::find(_Invited.begin(), _Invited.end(), user);
    if (_room_password_active)
        throw InvalidPasswordException();
    _Clients.push_back(user);
    _user_size++;
    if (it != _Invited.end())
        _Invited.erase(it);
    user->addChannel(_name);
};

void    Channel::join(Client *user, std::string password)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
    {
        throw MemberAlreadyHereException();
    }
    if (_invite_only)
    {
        if (std::find(_Invited.begin(), _Invited.end(), user) == _Invited.end())
            throw MissingInviteException();
    }

    if (_user_limit != 0 && _user_size >= _user_limit)
        throw ChannelFullException();
    std::vector<Client*>::iterator it = std::find(_Invited.begin(), _Invited.end(), user);
    if (_room_password_active && hash_password(password) != _room_password)
        throw InvalidPasswordException();
    _Clients.push_back(user);
    _user_size++;
    if (it != _Invited.end())
        _Invited.erase(it);
    user->addChannel(_name);
};

void    Channel::quit(Client *user, std::string msg)
{
    std::vector<Client*>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user);
    if (it == _Clients.end())
        throw UserNotInChannelException();
    _Clients.erase(it);
    it = std::find(_Admins.begin(), _Admins.end(), user);
    if (it != _Admins.end())
        _Admins.erase(it);
    _user_size--;
    if (_user_size == 1)
        _Admins.push_back(_Clients.front());
    user->removeChannel(_name);
};

void Channel::kick(Client *user, Client *target)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end())
        throw UserIsNotAdminException();
    std::vector<Client*>::iterator it = std::find(_Clients.begin(), _Clients.end(), target);
    if (it == _Clients.end())
        throw MemberNotFoundException();
    _Clients.erase(it);
    it = std::find(_Admins.begin(), _Admins.end(), target);
    if (it != _Admins.end())
        _Admins.erase(it);
    target->removeChannel(_name);
     _user_size--;
}

void    Channel::editTopic(Client *user, std::string topic)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw UserIsNotAdminException();
    _topic = topic;
};

void    Channel::editInvite_only(Client *user)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw UserIsNotAdminException();
    _invite_only = !_invite_only;
};

void    Channel::editTopic_admin_only(Client *user)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw UserIsNotAdminException();
    _topic_admin_only = !_topic_admin_only;
};

void    Channel::set_password(Client *user, std::string password)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw UserIsNotAdminException();
    _room_password = hash_password(password);
    _room_password_active = true;
};

void    Channel::add_Admin(Client *user, Client *target)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Clients.end())
        throw UserIsNotAdminException();
    if (std::find(_Clients.begin(), _Clients.end(), target) == _Clients.end())
        throw MemberNotFoundException();
    if (std::find(_Admins.begin(), _Admins.end(), target) == _Admins.end())
        _Admins.push_back(target);
};

void    Channel::editUser_limit(Client *user)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw UserIsNotAdminException();
    _user_limit = !_user_limit;
};

void    Channel::broadcast(Client *sender, std::string message)
{
    //if (std::find(_Clients.begin(), _Clients.end(), sender) == _Clients.end())
    //    throw UserNotInChannelException();
    for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
    {
        if (*it != sender)
            sender->sendMessage(*(*it), message);
    }
};

void    Channel::add_Invited(Client *user, Client *target)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw UserNotInChannelException();
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _invite_only)
        throw UserIsNotAdminException();
    if (std::find(_Clients.begin(), _Clients.end(), target) != _Clients.end())
        throw MemberAlreadyHereException();
    if (std::find(_Invited.begin(), _Invited.end(), target) != _Invited.end())
        throw UserAlreadyInvitedException();
    _Invited.push_back(target);
};

std::string Channel::getName() const
{
    return _name;
}

std::vector<Client*> Channel::getClients()
{
    return _Clients;
};

std::vector<Client*> Channel::getAdmins()
{
    return _Admins;
};

Client  *Channel::getClients(Client *user)
{
    std::vector<Client*>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user);
    if (it == _Clients.end())
        throw MemberNotFoundException();
    return (user);
};

Client  *Channel::getAdmins(Client *user)
{
    std::vector<Client*>::iterator it;
    it = std::find(_Admins.begin(), _Admins.end(), user);
    if (it == _Admins.end())
        throw UserIsNotAdminException();
    return (user);
};

bool    Channel::isTopicAdmin()
{
    return _topic_admin_only;
};

int Channel::ChannelFullException::errorCode() const
{
    return (471);
};

int Channel::InvalidPasswordException::errorCode() const
{
    return (475);
};

int Channel::MissingInviteException::errorCode() const
{
    return (473);
};

int Channel::MissingAuthorizationException::errorCode() const
{
    return (481);
};

int Channel::MemberNotFoundException::errorCode() const
{
    return (401);
};

int Channel::MemberAlreadyHereException::errorCode() const
{
    return (0);
};

int Channel::UserAlreadyInvitedException::errorCode() const
{
    return (0);
};

int Channel::UserIsNotAdminException::errorCode() const
{
    return (0);
};