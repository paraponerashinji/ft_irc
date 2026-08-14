/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 12:26:19 by aharder           #+#    #+#             */
/*   Updated: 2026/08/14 14:12:44 by aharder          ###   ########.fr       */
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
    std::string password = msg.substr(4);
    if (password == _server->getPassword())
        _server->create_client(_user);
};

void Nick::exec(std::string msg)
{
    std::string name = msg.substr(5);
    _user.setNick(name);
};

void User::exec(std::string msg)
{
    std::string username = msg.substr(5);
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
    std::string txt = msg.substr(8);
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
                _server.ircERROR(e.errorCode());
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
                _server.ircERROR(e.errorCode());
            }
        }
    }
};

