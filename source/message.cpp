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
    std::string password = msg.substr(msg.find(' '));
    if (password == _server->getPassword())
        _server->create_client(_user);
};

void Nick::exec(std::string msg)
{
    std::string name = msg.substr(msg.find(' '));
    _user.setNick(name);
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
    int buffer = 0;
    text.erase(std::remove(text.begin(), text.end(), ' '));
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == ',')
            targets.push_back(text.substr(buffer, i));
        buffer = i;
    }
    targets.push_back(text.substr(buffer, text.end));
    for (int i = 0; i < targets.length(); i++)
    {
        for (std::string:iterator it = targets[i].begin(); it != targets[i].end(); ++it)
            if (*it != ' ')
                break;
        if (it == targets[i].end())
            throw InvalidTargetInputException();
    }
    return targets;
}

void Privmsg::exec(std::string msg)
{
    std::string txt = msg.substr(msg.find(' '));
    std::string content = txt.substr(txt.find(':') + 1, txt.end());
    std::vector<std::string> targets = split(txt.substr(txt.find(':') - 1));
    for (int i = 0; i < targets.end(); i++)
    {
        if (targets[i][0] == '&' || targets[i][0] == '#')
        {
            targets[i].erase(1);
            try
            {
                _server.getChannel(targets[i]).broadcast(_user, content);
            }
            catch (std::exception &e)
            {
                _server.ircERROR(_user, e.errorCode());
            }
        }
        else
        {
            try
            {
                _server.getClients(targets[i]).sendMessage(_user, content);
            }
            catch (std::exception &e)
            {
                _server.ircERROR(_user, e.errorCode());
            }
        }
    }
};

void Quit::exec(std::string msg)
{
    
};

void Join::exec(std::string msg)
{
    std::string raw_msg = msg.substr(msg.find(' '));
    std::string channel = raw_msg.substr(raw_msg.begin(), raw_msg.find(' '));
    raw_msg = raw_msg.substr(raw_msg.find(' ') + 1);
    try
    {
        if (raw_msg.empty())
            _server.getChannel(channel).join(_server.getClients(_user));
        else
            _server.getChannel(channel).join(_server.getClients(_user), raw_msg);
    }
    catch (std::exception &e)
    {
        _server.ircERROR(_user, e.errorCode());
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