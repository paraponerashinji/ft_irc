#include "server.hpp"
#include "client.hpp"
#include "message.hpp"
#include "channel.hpp"

void    Server::ircERROR(Client *user, int code)
{
    (void)user;
    (void)code;
};

void Server::receiveMessage(Client &sender, std::string message)
{
    if (message.empty())
        return;
    if (message[message.size()] == '\r')
        message.erase(message.size());

    std::string cmd;
    std::string params;
    std::istringstream iss(message);
    iss >> cmd;
    std::getline(iss, params);
    if (!params.empty() && params[0] == ' ')
        params.erase(0, 1);
    std::string upper = cmd;
    for (size_t i = 0; i < upper.size(); ++i)
        upper[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(upper[i])));
    if (upper == "PASS")
    {
        //Pass pass(this, &sender, params);
        return;
    }
    else if (upper == "NICK")
    {
        //Nick nick(this, &sender, params);
        return;
    }
    else if (upper == "USER")
    {
        //User user(this, &sender, params);
        return;
    }
    else if (upper == "JOIN")
    {
        //Join join(this, &sender, params);
        return;
    }
    else if (upper == "PRIVMSG")
    {
        //Privmsg privmsg(this, &sender, params);
        return;
    }
    else if (upper == "KICK")
    {
        //Kick kick(this, &sender, params);
        return;
    }
    else if (upper == "INVITE")
    {
        //Invite invite(this, &sender, params);
        return;
    }
    else if (upper == "TOPIC")
    {
        //Topic topic(this, &sender, params);
        return;
    }
    else if (upper == "MODE")
    {
        //Mode mode(this, &sender, params);
        return;
    }
    else if (upper == "QUIT")
    {
        //Quit quit(this, &sender, params);
        return;
    }
    else
    {
        ircERROR(&sender, 421);
    }
};