#include "message.hpp"

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
    Message vanilla(sender, text);
    std::string command = vanilla.split_message()[0];
    int i = 0;
    while (i < 9)
    {
        if (command = commands[i])
            break;
    }
    switch (i)
	{
	case 0:
		create_client(vanilla.get_message());
		break;
	case 1:
        if (vanilla.split_message()[1][0] == "&" || vanilla.split_message()[1][0] == "#")
            Channel_message sendchannel(vanilla);
        else
            Private_message sendpv(vanilla);
		break;
	case 2:
        vanilla.join();
		break;
	case 3:
        create_client(NICK, vanilla.get_message());
		break;
    case 4:
        create_client(USER, vanilla.get_message());
		break;
    case 5:
        Target_command kickcommand(vanilla);
		break;
    case 6:
        Target_command invitecommand(vanilla);
		break;
    case 7:
        Topic_command topic(vanilla);
		break;
    case 8:
        Mode_command mode(vanilla);
		break;
    case 9:
        vanilla.message_error();
		break;
	}
}