#include "../include/exception.hpp"

void    Server::ircERROR(Client *user, std::string text)
{
    sendMessage(user, text);
};

IrcException::IrcException()
{
}

const char* IrcException::what() const throw()
{
    if (message.empty())
        return "Unknown Error";
    return message;
}

int ERR_NOSUCHNICK::errorCode() const { return 401; }
int ERR_NOSUCHCHANNEL::errorCode() const { return 403; }
int ERR_TOOMANYTARGETS::errorCode() const { return 407; }
int ERR_NOTEXTTOSEND::errorCode() const { return 412; }
int ERR_UNKNOWNCOMMAND::errorCode() const { return 421; }
int ERR_NONICKNAMEGIVEN::errorCode() const { return 431; }
int ERR_ERRONEUSNICKNAME::errorCode() const { return 432; }
int ERR_NICKNAMEINUSE::errorCode() const { return 433; }
int ERR_NICKCOLLISION::errorCode() const { return 436; }
int ERR_USERNOTINCHANNEL::errorCode() const { return 441; }
int ERR_NOTONCHANNEL::errorCode() const { return 442; }
int ERR_USERONCHANNEL::errorCode() const { return 443; }
int ERR_NOLOGIN::errorCode() const { return 444; }
int ERR_NOTREGISTERED::errorCode() const { return 451; }
int ERR_NEEDMOREPARAMS::errorCode() const { return 461; }
int ERR_ALREADYREGISTRED::errorCode() const { return 462; }
int ERR_PASSWDMISMATCH::errorCode() const { return 464; }
int ERR_KEYSET::errorCode() const { return 467; }
int ERR_CHANNELISFULL::errorCode() const { return 471; }
int ERR_UNKNOWNMODE::errorCode() const { return 472; }
int ERR_INVITEONLYCHAN::errorCode() const { return 473; }
int ERR_BADCHANNELKEY::errorCode() const { return 475; }
int ERR_CHANOPRIVSNEEDED::errorCode() const { return 482; }

ERR_NOSUCHNICK::ERR_NOSUCHNICK(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :No such nick/channel";
    message = oss.str();
};
ERR_NOSUCHCHANNEL::ERR_NOSUCHCHANNEL(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :No such channel";
    message = oss.str();
};

ERR_TOOMANYTARGETS::ERR_TOOMANYTARGETS(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Duplicate recipients. No message delivered";
    message = oss.str();
};

ERR_NOTEXTTOSEND::ERR_NOTEXTTOSEND(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " :No text to send";
    message = oss.str();
};

ERR_UNKNOWNCOMMAND::ERR_UNKNOWNCOMMAND(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Unknown command";
    message = oss.str();
};

ERR_NONICKNAMEGIVEN::ERR_NONICKNAMEGIVEN(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " :No nickname given";
    message = oss.str();
};

ERR_ERRONEUSNICKNAME::ERR_ERRONEUSNICKNAME(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Erroneus nickname";
    message = oss.str();
};

ERR_NICKNAMEINUSE::ERR_NICKNAMEINUSE(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Nickname is already in use";
    message = oss.str();
};

ERR_NICKCOLLISION::ERR_NICKCOLLISION(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Nickname collision KILL";
    message = oss.str();
};

ERR_USERNOTINCHANNEL::ERR_USERNOTINCHANNEL(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :They aren't on that channel";
    message = oss.str();
};

ERR_NOTONCHANNEL::ERR_NOTONCHANNEL(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :You're not on that channel";
    message = oss.str();
};

ERR_USERONCHANNEL::ERR_USERONCHANNEL(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :is already on channel"
    message = oss.str();
};

ERR_NOLOGIN::ERR_NOLOGIN(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << " User not logged in";
    message = oss.str();
};

ERR_NOTREGISTERED::ERR_NOTREGISTERED(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " :You have not registered";
    message = oss.str();
};

ERR_NEEDMOREPARAMS::ERR_NEEDMOREPARAMS(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Not enough parameters";
    message = oss.str();
};

ERR_ALREADYREGISTRED::ERR_ALREADYREGISTRED(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " :You may not reregister";
    message = oss.str();
};

ERR_PASSWDMISMATCH::ERR_PASSWDMISMATCH(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " :Password incorrect"; 
    message = oss.str();
};

ERR_KEYSET::ERR_KEYSET(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Channel key already set";
    message = oss.str();
};

ERR_CHANNELISFULL::ERR_CHANNELISFULL(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Cannot join channel (+l)";
    message = oss.str();
};

ERR_UNKNOWNMODE::ERR_UNKNOWNMODE(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :is unknown mode char to me";
    message = oss.str();
};

ERR_INVITEONLYCHAN::ERR_INVITEONLYCHAN(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Cannot join channel (+i)":
    message = oss.str();
};

ERR_BADCHANNELKEY::ERR_BADCHANNELKEY(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Cannot join channel (+k)";
    message = oss.str();
};

ERR_CHANOPRIVSNEEDED::ERR_CHANOPRIVSNEEDED(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :You're not channel operator";
    message = oss.str();
};

ERR_BADCHANMASK::ERR_BADCHANMASK(Client *User, const std::string &target): IrcException()
{
    std::ostringstream oss;
    oss << errorCode() << " " << User->getNickname() << " " << target << " :Bad Channel Mask";
    message = oss.str();
};