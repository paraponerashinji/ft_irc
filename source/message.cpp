/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 12:26:19 by aharder           #+#    #+#             */
/*   Updated: 2026/08/14 17:59:37 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

Message::Message(Server *server, Client user, std::string text): _server(server), _user(user), _text(text)
{
    exec(_text);
}

Message::~Message()
{
}

void Pass::exec(std::string msg)
{
    std::vector<Client>::iterator it;
    it = std::find(_server._Clients.begin(), _server._Clients.end(), user);
    if (it != _server._Clients.end())
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
};

void Nick::exec(std::string msg)
{
    std::string nickname = "";
    if (!msg.empty()) {
        size_t space = msg.find(' ');
        if (space != std::string::npos)
            nickname = msg.substr(space + 1);
    }
    _user.setNickname(nickname);
};

void User::exec(std::string msg)
{
    std::string username = msg.substr(msg.find(' '));
    if (_user.getUsername() != NULL)
            throw UsernameAlreadySetException();
    for (std::vector<Client>::iterator it = _server.getClients.begin(); it != _server.getClients.end(); ++it)
    {
        if (*it.getUsername() == username)
            throw UsernameAlreadyTakenException();
    }
    _user.setUsername(username);
};

std::vector<std::string> Privmsg::split(std::string text)
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
    std::string txt = msg.substr(msg.find(' '));
    std::string content = txt.substr(txt.find(':') + 1, txt.end());
    std::vector<std::string> targets = split(txt.substr(txt.find(':') - 1));
    for (int i = 0; i < targets.size(); ++i)
    {
        if (targets[i][0] == '&' || targets[i][0] == '#')
        {
            targets[i].erase(1);
            try
            {
                _server->getChannel(targets[i]).broadcast(_user, content);
            }
            catch (std::exception &e)
            {
                _server->ircERROR(_user, e.errorCode());
            }
        }
        else
        {
            try
            {
                _server->getClients(targets[i]).sendMessage(_user, content);
            }
            catch (std::exception &e)
            {
                _server->ircERROR(_user, e.errorCode());
            }
        }
    }
};

void Quit::exec(std::string msg)
{
    (void)msg;
    std::vector<std::string> channels = _user.getChannels();
    for (size_t i = 0; i < channels.size(); ++i)
    {
        try
        {
            Channel &channel = _server->getChannel(channels[i]);
            channel.quit(&_user);
        }
        catch (std::exception &e)
        {
            _server.ircERROR(_user, e.errorCode());
        }
    }
    _server->removeClient(_user.getFd());
};

void Part::exec(std::string msg)
{
    std::string channel = msg;
    size_t space = channel.find(' ');
    if (space != std::string::npos)
        channel = channel.substr(space + 1);
    Channel &chan = _server->getChannel(channel);
    chan.quit(&_user);
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
        Channel &chan = _server->getChannel(channel);
        if (key.empty())
            chan.join(&_user);
        else
            chan.join(&_user, key);
    }
    catch (std::exception &e)
    {
        _server->ircERROR(_user, e.errorCode());
    }
};

void Topic::exec(std::string msg)
{
    std::string raw_msg = msg.substr(msg.find(' '));
    std::string channel = raw_msg.substr(raw_msg.begin(), raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    try
    {
        _server.getChannel(channel).getClients(_user);
        if (_server.getChannel(channel).isTopicAdmin())
            _server.getChannel(channel).getAdmins(_user);
        _server.getChannel(channel).editTopic(raw_msg);
    }
    catch (std::exception &e)
    {
        _server.ircERROR(_user, e.errorCode());
    }
};

void Kick::exec(std::string msg)
{
    std::string raw_msg = msg.substr(msg.find(' '));
    std::string channel = raw_msg.substr(raw_msg.begin(), raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    std::string target = raw_msg.substr(raw_msg.begin(), raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    try
    {
        _server.getChannel(channel).getAdmins(_user);
        _server.getChannel(channel).kick(_server.getClients(target), raw_msg);
    }
    catch (std::exception &e)
    {
        _server.ircERROR(_user, e.errorCode());
    }
};

void Invite::exec(std::string msg)
{
    std::string raw_msg = msg.substr(msg.find(' '));
    std::string target = raw_msg.substr(raw_msg.begin(), raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    std::string channel = raw_msg.substr(raw_msg.begin(), raw_msg.find(' '));
    try
    {
        _server.getChannel(channel).getClients(_user);
        _server.getChannel(channel).add_Invited(_server.getClients(target));
    }
    catch (std::exception &e)
    {
        _server.ircERROR(_user, e.errorCode());
    }
};

void Mode::exec(std::string)
{
    //  TODO
};