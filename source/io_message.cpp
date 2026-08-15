#ifndef IO_MSG
#define IO_MSG
#include "../include/message.hpp"
#include "../include/server.hpp"

Server::receive_message(Client sender, std::string text)
{
    std::string commands[9] =
    {
        "PASS",
        "PRIVMSG",
        "JOIN",
        "NICK",
        "USER",
        "KICK",
        "INVITE",
        "TOPIC",
        "MODE"
    }
    std::stringstream ss(line)
    std::string command;
    ss >> command;
    int i = 0;
    while (i < 9)
    {
        if (command = commands[i])
            break;
    }
    switch (i)
	{
	case 0:
        Pass(*this, sender, text);
		break;
	case 1:
        Privmsg(*this, sender, text);
		break;
	case 2:
        Join(*this, sender, text);
		break;
	case 3:
        Nick(*this, sender, text);
		break;
    case 4:
        User(*this, sender, text);
		break;
    case 5:
        Kick(*this, sender, text);
		break;
    case 6:
        Invite(*this, sender, text);
		break;
    case 7:
        Topic(*this, sender, text);
		break;
    case 8:
        Mode(*this, sender, text);
		break;
    case 9:
        ircERROR(*this, sender, 421);
		break;
	}
};
#endif