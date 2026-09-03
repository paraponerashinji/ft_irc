/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 12:26:19 by aharder           #+#    #+#             */
/*   Updated: 2026/09/03 13:49:20 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"
#include "client.hpp"
#include "server.hpp"
#include "channel.hpp"
#include "exception.hpp"

Message::Message(Server *server, Client *user, std::string text): _server(server), _user(user), _text(text)
{
};

Message::~Message()
{
};

std::string Join::getPassword()
{
    return _password;
};

void Pass::exec(std::string msg)
{
    (void)msg;
    /*
    std::vector<Client>::iterator it;
    it = std::find(_server->getClients().begin(), _server->getClients().end(), user);
    if (it != _server->getClients().end())
        throw ClientExistAlreadyException();
    std::string password = "";
    if (!msg.empty())
    {
        size_t space = msg.find(' ');
        if (space != std::string::npos)
            password = msg.substr(space + 1);
    }
    if (password == _server->getPassword())
        _server->create_client(_user);
    */
};

void Nick::exec(std::string msg)
{
    std::string nickname = "";
    if (!msg.empty()) {
        size_t space = msg.find(' ');
        if (space != std::string::npos)
            nickname = msg.substr(space + 1);
    }
    _user->setNickname(nickname);
};

void User::exec(std::string msg)
{
    std::string username = msg.substr(msg.find(' '));
    if (!_user->getUsername().empty())
            throw UserNameAlreadySetException();
    for (std::vector<Client>::iterator it = _server->getClients()->begin(); it != _server->getClients()->end(); ++it)
    {
        if ((*it).getUsername() == username)
            throw UserNameAlreadyTakenException();
    }
    _user->setUsername(username);
};

int User::UserNameAlreadySetException::errorCode() const
{
    return (462);
};

int User::UserNameAlreadyTakenException::errorCode() const
{
    return (475);
};


std::vector<std::string> Privmsg::split(std::string msg)
{
    std::vector<std::string> targets;
    size_t sep = msg.find(' ');
    std::string rest = msg.substr(sep + 1);
    size_t colon = rest.find(':');
    std::string target_part = rest.substr(0, colon);
    std::string content = rest.substr(colon + 1);
    std::stringstream ss(target_part);
    std::string token;
    while (std::getline(ss, token, ','))
    {
        targets.push_back(token);
    }
    return targets;
};

void Privmsg::exec(std::string msg)
{
    std::string txt = msg.substr(msg.find(' '), msg.find(':'));
    std::string content = txt.substr(txt.find(':') + 1);
    std::vector<std::string> targets = split(txt.substr(txt.find(':') - 1));
    for (size_t i = 0; i < targets.size(); ++i)
    {
        if (targets[i][0] == '&' || targets[i][0] == '#')
        {
            targets[i].erase(1);
            try
            {
                Channel *channel = _server->getChannel(targets[i]);
                channel->broadcast(_user, content);
            }
            catch (IrcException &e)
            {
                _server->ircERROR(_user, e.errorCode());
            }
        }
        else
        {
            try
            {
                Client *client = _server->getClientPtr(targets[i]);
                _user->sendMessage(client, content);
            }
            catch (IrcException &e)
            {
                _server->ircERROR(_user, e.errorCode());
            }
        }
    }
};

void Quit::exec(std::string msg)
{
    (void)msg;
    std::vector<std::string> channels = _user->getChannels();
    for (size_t i = 0; i < channels.size(); ++i)
    {
        try
        {
            Channel *channel = _server->getChannel(channels[i]);
            channel->quit(_user);
        }
        catch (IrcException &e)
        {
            _server->ircERROR(_user, e.errorCode());
        }
    }
    _server->removeClient(_user->getFd());
};

void Part::exec(std::string msg)
{
    std::string channel = msg;
    size_t space = channel.find(' ');
    if (space != std::string::npos)
        channel = channel.substr(space + 1);
    Channel *chan = _server->getChannel(channel);
    chan->quit(_user);
};

void Join::exec(std::string msg)
{
    try
    {
        std::string params = msg;
        size_t first = params.find(' ');
        if (first != std::string::npos)
            params = params.substr(first + 1);
        std::string channel = params;
        std::string key = "";
        size_t space = channel.find(' ');
        if (space != std::string::npos)
        {
            key = channel.substr(space + 1);
            channel = channel.substr(0, space);
        }
        Channel *chan = _server->getChannel(channel);
        if (key.empty())
            chan->join(_user);
        else
            chan->join(_user, *this);
    }
    catch (IrcException &e)
    {
        _server->ircERROR(_user, e.errorCode());
    }
};

void Topic::exec(std::string msg)
{
    std::string raw_msg = msg.substr(msg.find(' '));
    std::string channel = raw_msg.substr(0, raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    try
    {
        Channel *chan = _server->getChannel(channel);
        chan->getClients(_user);
        if (chan->isTopicAdmin())
            chan->getAdmins(_user);
        chan->editTopic(raw_msg);
    }
    catch (IrcException &e)
    {
        _server->ircERROR(_user, e.errorCode());
    }
};

void Kick::exec(std::string msg)
{
    std::string raw_msg = msg.substr(msg.find(' '));
    std::string channel = raw_msg.substr(0, raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    std::string target = raw_msg.substr(0, raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    try
    {
        Channel *chan = _server->getChannel(channel);
        chan->getAdmins(_user);
        chan->kick(_server->getClientPtr(target), raw_msg);
    }
    catch (IrcException &e)
    {
        _server->ircERROR(_user, e.errorCode());
    }
};

void Invite::exec(std::string msg)
{
    std::string raw_msg = msg.substr(msg.find(' '));
    std::string target = raw_msg.substr(0, raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    std::string channel = raw_msg.substr(0, raw_msg.find(' '));
    try
    {
        Channel *chan = _server->getChannel(channel);
        chan->getClients(_user);
        chan->add_Invited(_server->getClientPtr(target));
    }
    catch (IrcException &e)
    {
        _server->ircERROR(_user, e.errorCode());
    }
};

void Mode::exec(std::string)
{
    //  TODO
};