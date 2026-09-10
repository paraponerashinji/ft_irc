#include "../include/exception.hpp"
#include "../include/channel.hpp"
#include "../include/client.hpp"
#include <string>
std::vector<std::string> Server::Parse_Line(std::string text)
{
    std::stringstream ss(text);
    std::vector<std::string> params;
    std::string word;
    while (ss >> word)
    {
        if (!word.empty() && word[0] == ':')
        {
            std::string rest;
            std::getline(ss, rest);
            if (!rest.empty() && rest[0] == ' ')
                rest.erase(0, 1);
            std::string combined = word.substr(1);
            if (!rest.empty())
                combined += std::string(" ") + rest;

            params.push_back(combined);
            break;
        }
        params.push_back(word);
    }
    return params;
};

int stoi98(const std::string& str)
{
    char* end;
    long value = std::strtol(str.c_str(), &end, 10);

    if (end == str.c_str())
        throw std::invalid_argument("invalid integer");

    if (*end != '\0')
        throw std::invalid_argument("invalid integer");

    if (value < -2147483648L || value > 2147483647L)
        throw std::out_of_range("integer out of range");

    return static_cast<int>(value);
}

bool    Server::isFullyRegistered(Client *sender)
{
    if (sender->getHostname().empty())
        return false;
    if (sender->getNickname().empty())
        return false;
    if (sender->getUsername().empty())
        return false;
    if (!sender->isRegistered())
        return false;
    return true;
};
void    Server::join(Client *sender, std::string text)
{
    if (!isFullyRegistered(sender))
        throw ERR_NOTREGISTERED(sender, "");
    std::vector<std::string> params = Parse_Line(text);
    if (params.size() < 1)
        throw ERR_NEEDMOREPARAMS(sender, "JOIN");
    std::vector<std::string> channels;
    std::istringstream chss(params[0]);
    std::string ch;
    while (std::getline(chss, ch, ','))
    {
        if (!ch.empty())
            channels.push_back(ch);
    }
    std::vector<std::string> keys;
    if (params.size() >= 2)
    {
        std::istringstream kss(params[1]);
        std::string key;
        while (std::getline(kss, key, ','))
            keys.push_back(key);
    }
    for (size_t j = 0; j < channels.size(); j++)
    {
        if (channels[j].empty())
            throw ERR_NOSUCHCHANNEL(sender, "");
        if (channels[j][0] != '#' && channels[j][0] != '&')
            throw ERR_NOSUCHCHANNEL(sender, channels[j]);;
        try
        {
            Channel *channel = getChannel(channels[j]);
            if (channel == NULL)
               throw ERR_NOSUCHCHANNEL(sender, channels[j]);
            if (j < keys.size() && !keys[j].empty())
                channel->join(sender, keys[j]);
            else
                channel->join(sender);
            std::ostringstream output;
            output << "JOIN " << channel->getName();
            channel->broadcast(sender, output.str());
        }
        catch (IrcException &e)
        {
            if (e.errorCode() == 403)
            {
                if (keys.empty() || keys[j].empty())
                    createChannel(channels[j], sender);
                else
                    createChannel(channels[j], keys[j], sender);
                continue;
            }
            else
                throw;
        }
    }
};

void    Server::part(Client *sender, std::string text)
{
    if (!isFullyRegistered(sender))
        throw ERR_NOTREGISTERED(sender,"");
    std::vector<std::string> params = Parse_Line(text);
    if (params.size() < 1)
        throw ERR_NEEDMOREPARAMS(sender, "PART");
    std::vector<std::string> channels;
    std::istringstream chss(params[0]);
    std::string ch;
    while (std::getline(chss, ch, ','))
    {
        if (!ch.empty())
            channels.push_back(ch);
    }
    std::vector<std::string> args;
    if (params.size() >= 2)
    {
        std::istringstream kss(params[1]);
        std::string key;
        while (std::getline(kss, key, ','))
            args.push_back(key);
    }
    for (size_t j = 0; j < channels.size(); j++)
    {
        if (channels[j][0] != '#' && channels[j][0] != '&')
            throw ERR_NOSUCHCHANNEL(sender, channels[j]);;
        if (channels[j].empty())
            throw ERR_NOSUCHCHANNEL(sender, "");;
        Channel *channel = getChannel(channels[j]);
        channel->quit(sender);
        std::ostringstream output;
        if (args.empty())
            output << "PART " << channel->getName();
        else
            output << "PART " << channel->getName() << " :" << args[0];
        channel->broadcast(sender, output.str());
    }
};

void    Server::privmsg(Client *sender, std::string text)
{
    if (!isFullyRegistered(sender))
        throw ERR_NOTREGISTERED(sender, "");
    std::vector<std::string> params = Parse_Line(text);
    if (params.size() < 2)
        throw ERR_NEEDMOREPARAMS(sender, "PRIVMSG");
    if (params[1].empty())
        throw ERR_NOTEXTTOSEND(sender, "");
    std::vector<std::string> channels;
    std::istringstream chss(params[0]);
    std::string ch;
    while (std::getline(chss, ch, ','))
    {
        if (!ch.empty())
            channels.push_back(ch);
    }
    for (size_t j = 0; j < channels.size(); j++)
    {
        if (channels[j][0] == '#' || channels[j][0] == '&')
        {
            try
            {
                Channel *channel = getChannel(channels[j]);
                Client *c = channel->getClients(sender);
                if (c == NULL)
                    throw ERR_NOTONCHANNEL(sender, channels[j]);
                if (params[1].empty())
                    throw ERR_NOTEXTTOSEND(sender, "");
                std::ostringstream output;         
                output << "PRIVMSG " << channel->getName() << " " << params[1];
                channel->broadcast(sender, output.str());
            }
            catch (IrcException &e)
            {
                if (e.errorCode() == 403)
                    throw ERR_NOSUCHNICK(sender, channels[j]);
                throw ;
            }
        }
        else
        {
            Client *client = getClientPtr(channels[j]);
            if (client == NULL)
                throw ERR_NOSUCHNICK(sender, channels[j]);
            if (params[1].empty())
                throw ERR_NOTEXTTOSEND(sender, "");
            std::ostringstream output;          
            output << "PRIVMSG " << client->getNickname() << " " << params[1];
            sender->sendMessage(client, output.str());
        }
    }
};

void    Server::kick(Client *sender, std::string text)
{
    if (!isFullyRegistered(sender))
        throw ERR_NOTREGISTERED(sender, "");
    std::vector<std::string> params = Parse_Line(text);
    if (params.size() < 2)
        throw ERR_NEEDMOREPARAMS(sender, "KICK");
    std::vector<std::string> channels;
    std::istringstream chss(params[0]);
    std::string ch;
    while (std::getline(chss, ch, ','))
    {
        if (!ch.empty())
            channels.push_back(ch);
    }
    std::vector<std::string> users;
    if (params.size() >= 2)
    {
        std::istringstream kss(params[1]);
        std::string key;
        while (std::getline(kss, key, ','))
            users.push_back(key);
    }
    for (size_t j = 0; j < channels.size(); j++)
    {
        Channel *channel = getChannel(channels[j]);
        for (size_t y = 0; y < users.size(); y++)
        {
            Client *target = channel->getClient(users[y]);
            if (target == NULL)
                throw ERR_NOSUCHNICK(sender, users[y]);
            channel->kick(sender, target);
            std::ostringstream output;
            if (params.size() >= 3)
            {
                output << "KICK " << channel->getName() << " " << target->getNickname() << " " << params[2];
                channel->broadcast(sender, output.str());
                sender->sendMessage(target, output.str());
                continue ;
            }
            output << "KICK " << channel->getName() << " " << target->getNickname();
            channel->broadcast(sender, output.str());
            sender->sendMessage(target, output.str());
        }
    }
};

void    Server::invite(Client *sender, std::string text)
{
    if (!isFullyRegistered(sender))
        throw ERR_NOTREGISTERED(sender, "");
    std::vector<std::string> params = Parse_Line(text);
    if (params.size() < 2)
        throw ERR_NEEDMOREPARAMS(sender, "INVITE");
    if (params[0][0] == '#' || params[0][0] == '&')
        throw ERR_NOSUCHNICK(sender, params[0]);
    if (params[1][0] != '#' && params[1][0] != '&')
        throw ERR_NOSUCHCHANNEL(sender, params[1]);
    try
    {
        Channel *channel = getChannel(params[1]);
        if (channel = NULL)
            throw ERR_NOSUCHCHANNEL(sender, params[0]);
        Client *target = getClientPtr(params[0]);
        if (target == NULL)
            throw ERR_NOSUCHNICK(sender, params[0])
        channel->add_Invited(sender, target);
        std::ostringstream output;
        output << "INVITE " << target->getNickname() << " " << channel->getName();
        sender->sendMessage(target, output.str());
    }
    catch (IrcException &e)
    {
        if (e.errorCode() == 403)
        {
            Client *target = getClientPtr(params[0]);
            if (target == NULL)
                throw ERR_NOSUCHNICK(sender, params[0])
            std::ostringstream output;
            output << "INVITE " << target->getNickname() << " #" << params[1];
            sender->sendMessage(target, output.str());
        }
    }
};

void    Server::topic(Client *sender, std::string text)
{
    if (!isFullyRegistered(sender))
        throw ERR_NOTREGISTERED(sender, "");
    std::vector<std::string> params = Parse_Line(text);
    if (params.size() < 2)
        throw ERR_NEEDMOREPARAMS(sender, "TOPIC");
    if (params[0][0] != '#' && params[0][0] != '&')
        throw ERR_NOSUCHCHANNEL(sender, params[0]);
    Channel *channel = getChannel(params[0]);
    if (channel = NULL)
        throw ERR_NOSUCHCHANNEL(sender, params[0]);
    channel->editTopic(sender, params[1]);
    std::ostringstream output;
    output << "TOPIC " << channel->getName() << " :" << params[1];
    channel->broadcast(sender, output.str());
};

void    Server::mode(Client *sender, std::string text)
{
    bool    make = false;
    if (!isFullyRegistered(sender))
        throw ERR_NOTREGISTERED(sender, "");
    std::vector<std::string> params = Parse_Line(text);
    if (params.size() < 2)
        throw ERR_NEEDMOREPARAMS(sender, "MODE");
    if (params[0][0] != '#' && params[0][0] != '&')
        throw ERR_NOSUCHCHANNEL(sender, params[0]);
    Channel *channel = getChannel(params[0]);
    if (channel == NULL)
        throw ERR_NOSUCHCHANNEL(sender, params[0]);
    if (params[1][0] != '+' && params[1][0] != '-')
        throw ERR_UNKNOWNMODE(sender, std::string(1, params[1][0]));
    if (params[1][0] == '+')
        make = true;
    params[1].erase(0,1);
    size_t k = 2;
    for (size_t i = 0; i < params[1].size(); i++)
    {
        char mode[5] = { 'o', 't', 'i', 'k', 'l'};
        size_t j = 0;
        while (j < 5)
        {
            if (params[1][i] == mode[j])
                break;
            j++;
        }
        switch (j)
        {
            case 0:
            {
                if (params.size() < k)
                    throw ERR_NEEDMOREPARAMS(sender, "MODE");
                Client *c = getClientPtr(params[k]);
                if (c == NULL)
                    throw ERR_NOSUCHNICK(sender, params[k]);
                if (make)
                    channel->add_Admin(sender, c);
                else
                    channel->remove_Admin(sender, c);  
                k++;
                break;
            }
            case 1:
                channel->editTopic_admin_only(sender, make);
                break;
            case 2:
                channel->editInvite_only(sender, make);
                break;
            case 3:
                if (!make)
                    channel->remove_password(sender);
                else
                {
                    if (params.size() <= k)
                        throw ERR_NEEDMOREPARAMS(sender, "MODE");
                    channel->set_password(sender, params[k]);
                    k++;
                }
                break;
            case 4:
                if (params.size() < k || !make)
                    channel->editUser_limit(sender, 0);
                else if (params[k].find_first_not_of("0123456789") == std::string::npos)
                {
                    channel->editUser_limit(sender, stoi98(params[k]));
                    k++;
                }
                else
                    throw   ERR_NEEDMOREPARAMS(sender, "MODE");
                break;
            default:
                throw ERR_UNKNOWNMODE(sender, std::string(1, params[i][0]));
        }
    }
}