#include "../include/channel.hpp"
#include "../include/client.hpp"
#include "../include/server.hpp"
#include "../include/message.hpp"
#include "../include/exception.hpp"
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

Channel::Channel(Server *server, std::string name, Client *client): _server(server), _name(name)
{
    if (name.find_first_of("#") == std::npos)
        throw ERR_BADCHANMASK(client, name);
    if (name.find_first_of(" '") != std::npos)
        throw ERR_BADCHANMASK(client, name);
    _Admins.push_back(client);
    _Clients.push_back(client);
    _user_size = 1;
    client->addChannel(name);
    _topic = "";
    _room_password = "";
    _user_limit = 0;
    _invite_only = false;
    _topic_admin_only = false;
    _room_password_active = false;
};

Channel::Channel(Server *server, std::string name, std::string password, Client *client): _server(server), _name(name)
{
    if (name.find_first_of("#") == std::npos)
        throw ERR_BADCHANMASK(client, name);
    if (name.find_first_of(" '") != std::npos)
        throw ERR_BADCHANMASK(client, name);
    _Admins.push_back(client);
    _Clients.push_back(client);
    _user_size = 1;
    client->addChannel(name);
    _topic = "";
    _room_password = hash_password(password);
    _user_limit = 0;
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
        std::string t(user->getNickname());
        t.append(" ");
        t.append(_name);
        throw ERR_USERONCHANNEL(user, t);
    }
    if (_invite_only)
    {
        bool invited = false;
        std::cout << _Invited.size() << std::endl;
        for (std::vector<Client*>::iterator it = _Invited.begin(); it != _Invited.end(); ++it)
        {
            if (*it && (*it)->getNickname() == user->getNickname())
            {
                std::cout << (*it)->getNickname() << std::endl;
                invited = true;
                break;
            }
        }
        if (!invited)
            throw ERR_INVITEONLYCHAN(user, _name);
    }
    if (_user_limit != 0 && _user_size >= _user_limit)
        throw ERR_CHANNELISFULL(user, _name);
    
    if (_room_password_active)
        throw ERR_BADCHANNELKEY(user,_name);
    
    _Clients.push_back(user);
    _user_size++;
    user->addChannel(_name);
    std::vector<Client*>::iterator it = std::find(_Invited.begin(), _Invited.end(), user);
    if (it != _Invited.end())
        _Invited.erase(it);
};

void    Channel::join(Client *user, std::string password)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) != _Clients.end())
    {
        std::string t(user->getNickname());
        t.append(" ");
        t.append(_name);
        throw ERR_USERONCHANNEL(user, t);
    }
    if (_invite_only)
    {
        bool invited = false;
        for (std::vector<Client*>::iterator it = _Invited.begin(); it != _Invited.end(); ++it)
        {
            if (*it && (*it)->getNickname() == user->getNickname())
            {
                invited = true;
                break;
            }
        }
        if (!invited)
            throw ERR_INVITEONLYCHAN(user, _name);
    }
    if (_user_limit != 0 && _user_size >= _user_limit)
        throw ERR_CHANNELISFULL(user, _name);
    std::vector<Client*>::iterator it = std::find(_Invited.begin(), _Invited.end(), user);
    if (_room_password_active && hash_password(password) != _room_password)
        throw ERR_BADCHANNELKEY(user, _name);
    _Clients.push_back(user);
    _user_size++;
    if (it != _Invited.end())
        _Invited.erase(it);
    user->addChannel(_name);
};

void    Channel::quit(Client *user)
{
    std::vector<Client*>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user);
    if (it == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
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
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end())
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    std::vector<Client*>::iterator it = std::find(_Clients.begin(), _Clients.end(), target);
    if (it == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
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
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    _topic = topic;
};

void    Channel::editInvite_only(Client *user, bool value)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    _invite_only = value;
};

void    Channel::editTopic_admin_only(Client *user, bool value)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    _topic_admin_only = value;
};

void    Channel::set_password(Client *user, std::string password)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    _room_password = hash_password(password);
    _room_password_active = true;
};

void    Channel::add_Admin(Client *user, Client *target)
{
    if (target == NULL)
        throw ERR_NOSUCHNICK(user, )
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Clients.end())
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    if (std::find(_Clients.begin(), _Clients.end(), target) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), target) == _Admins.end())
        _Admins.push_back(target);
};

void    Channel::editUser_limit(Client *user, int value)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    _user_limit = value;
};

void    Channel::remove_Admin(Client *user, Client *target)
{
    std::vector<Client*>::iterator it;
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Clients.end())
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    if (std::find(_Clients.begin(), _Clients.end(), target) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    it = std::find(_Admins.begin(), _Admins.end(), target);
    if (it != _Admins.end())
        _Admins.erase(it);
};

void    Channel::remove_password(Client *user)
{
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _topic_admin_only)
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    _room_password_active = false;
};

void    Channel::broadcast(Client *sender, std::string message)
{
    if (std::find(_Clients.begin(), _Clients.end(), sender) == _Clients.end())
        throw ERR_NOTONCHANNEL(sender, _name);
    for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
    {
        if (*it != sender && it != _Clients.end())
            sender->sendMessage((*it), message);
    }
};

void    Channel::add_Invited(Client *user, Client *target)
{
    std::cout << "p";
    if (std::find(_Clients.begin(), _Clients.end(), user) == _Clients.end())
        throw ERR_NOTONCHANNEL(user, _name);
    if (std::find(_Admins.begin(), _Admins.end(), user) == _Admins.end() && _invite_only)
        throw ERR_CHANOPRIVSNEEDED(user, _name);
    if (std::find(_Clients.begin(), _Clients.end(), target) != _Clients.end())
        throw ERR_USERONCHANNEL(user, _name);
    if (std::find(_Invited.begin(), _Invited.end(), target) != _Invited.end())
        return ;
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

Client *Channel::getClient(std::string user)
{
    for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
    {
        if (*it && (*it)->getNickname() == user)
            return *it;
    }
    return NULL;
}

Client  *Channel::getClients(Client *user)
{
    std::vector<Client*>::iterator it;
    it = std::find(_Clients.begin(), _Clients.end(), user);
    if (it == _Clients.end())
        return NULL;
    return (user);
};

Client  *Channel::getAdmins(Client *user)
{
    std::vector<Client*>::iterator it;
    it = std::find(_Admins.begin(), _Admins.end(), user);
    if (it == _Admins.end())
        return NULL;
    return (user);
};

size_t  Channel::getUserLimit()
{
    return _user_limit;
};

bool    Channel::isTopicAdmin()
{
    return _topic_admin_only;
};