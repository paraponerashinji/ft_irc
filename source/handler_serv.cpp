/*void    handlePrivmsg(Client* sender, Channel* channel, const std::string& text) {
    std::string msg = ":" + sender->getPrefix() + " PRIVMSG " + channel->getName() + " :" + text + "\r\n";

    channel->broadcast(sender, msg);
}*/

#include "../include/server.hpp"
#include "../include/client.hpp"
#include "../include/exception.hpp"
#include "../include/channel.hpp"

void Server::sendWelcome(Client *client)
{
    std::ostringstream oss;

    oss << ":" << client->getServerIp() << " 001 " << client->getNickname() << " :Welcome to the Internet Relay Network " << client->getNickname() << "!" << client->getUsername() << "@" << client->getHostname() << "\r\n";
    sendMessage(*client, oss.str());
    oss.str("");
    oss.clear();
    oss << ":" << client->getServerIp() << " 002 " << client->getNickname() << " :Your host is " << client->getServerIp() << ", running ft_irc\r\n";
    sendMessage(*client, oss.str());
    oss.str("");
    oss.clear();
    oss << ":" << client->getServerIp() << " 003 " << client->getNickname() << " :This server was created today\r\n";
    sendMessage(*client, oss.str());
}

void Server::Pass(Client* client, const std::vector<std::string>& params)
{
    if (client->isRegistered())
        throw ERR_ALREADYREGISTRED(client, "");
    if (params.empty() || params[0].empty())
        throw ERR_NEEDMOREPARAMS(client, "PASS");
    if (params[0] != _password)
        throw ERR_PASSWDMISMATCH(client, "");
    client->setRegistered(true);
    if (isFullyRegistered(client))
        sendWelcome(client);
}

bool Server::isValidNickname(const std::string& nick) {
    if (nick.empty() || nick.length() > 9)
        return false;
    std::string invalidStart = "#$&:0123456789,?!@";
    if (invalidStart.find(nick[0]) != std::string::npos)
        return false;
    std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-[]\\`_^{|}";
    for (size_t i = 0; i < nick.length(); ++i) {
        if (validChars.find(nick[i]) == std::string::npos)
            return false;
    }
    return true;
}

void Server::broadcastToCommonChannels(Client* client, const std::string& message) {
    std::set<Client*> notifiedClients;
    const std::vector<std::string>& clientChanNames = client->getChannels();

    for (size_t i = 0; i < clientChanNames.size(); ++i) {
        Channel* chan = getChannel(clientChanNames[i]);
        if (chan) { // _channels[i]   _channels[i]->hasMember(client)) {
            // Récupère les membres du canal
            const std::vector<Client*>& members = chan->getClients();
            for (size_t j = 0; j < members.size(); ++j) {
                if (members[j] != client) {
                    notifiedClients.insert(members[j]); // Évite les doublons avec std::set
                }
            }
        }
    }

    // Envoie le message NICK aux utilisateurs uniques
    for (std::set<Client*>::iterator it = notifiedClients.begin(); it != notifiedClients.end(); ++it) {
        send((*it)->getFd(), message.c_str(), message.length(), MSG_NOSIGNAL);
    }
}

void Server::Nick(Client* client, const std::vector<std::string>& params)
{
    if (!client->isRegistered())
        throw ERR_NOTREGISTERED(client, "");
    if (params.empty() || params[0].empty())
        throw ERR_NONICKNAMEGIVEN(client, "");
    std::string newNick = params[0];
    if (!isValidNickname(newNick))
        throw ERR_ERRONEUSNICKNAME(client, newNick);
    Client* existingClient = getClientPtr(newNick);
    if (existingClient && existingClient != client)
        throw ERR_NICKNAMEINUSE(client, newNick);
    if (isFullyRegistered(client))
    {
        if (existingClient && existingClient != client)
            throw ERR_NICKNAMEINUSE(client, newNick);
        std::string oldPrefix = client->getPrefix();
        client->setNickname(newNick);
        std::string nickMsg = ":" + oldPrefix + " NICK " + newNick + "\r\n";
        sendMessage(*client, nickMsg);
        broadcastToCommonChannels(client, nickMsg);
    }
    else
    {
        client->setNickname(newNick);
        if (isFullyRegistered(client))
            sendWelcome(client);
    }
}


/*void Server::checkRegistration(Client* client) {
    // Si PASS est valide + NICK défini + USER défini + PAS ENCORE ENREGISTRÉ
    if (client->hasPassed() && !client->getNickname().empty() && client->hasUser() && !client->isRegistered()) {
        
        // Validation officielle de l'enregistrement
        client->setRegistered(true);

        // Envoi des messages d'accueil RFC (RPL_WELCOME, etc.)
        std::string welcome = ":" + _serverName + " 001 " + client->getNickname() + 
                              " :Welcome to the IRC Network " + client->getPrefix() + "\r\n";
        
        send(client->getFd(), welcome.c_str(), welcome.length(), MSG_NOSIGNAL);
        
        // (Optionnel) Tu peux aussi envoyer RPL_YOURHOST (002), RPL_CREATED (003), RPL_MYINFO (004)
    }
}*/

void Server::User(Client* client, const std::vector<std::string>& params) {
    if (!client->isRegistered())
        throw ERR_NOTREGISTERED(client, "");
    if (isFullyRegistered(client)) {
        throw ERR_ALREADYREGISTRED(client, "");
        return;
    }
    if (params.size() < 4) {
        throw ERR_NEEDMOREPARAMS(client, "USER");
        return;
    }
    client->setUsername(params[0]);
    client->setRealname(params[3]);
    if (isFullyRegistered(client))
        sendWelcome(client);
}



void Server::Quit(Client* client, const std::vector<std::string>& params) {
    std::string reason = "Quit: ";
    if (!params.empty() && !params[0].empty()) {
        reason += params[0];
    } else {
        reason += "Client Quit";
    }
    std::string quitMsg = ":" + client->getPrefix() + " QUIT :" + reason + "\r\n";
    std::ostringstream oss; 
    oss << ":" << client->getServerIp() << " ERROR :Closing Link" << std::endl;
    sendMessage(*client, oss.str());
    broadcastToCommonChannels(client, quitMsg);
    std::cout << BRED <<"Client disconnection, fd :" << client->getFd() << RESET << std::endl;
    const std::vector<std::string>& clientChanNames = client->getChannels();
    for (size_t i = 0; i < clientChanNames.size(); ++i) {
        Channel* chan = getChannel(clientChanNames[i]);
        if (chan) {
            _channels[i]->quit(client);
        }
    }
    close(client->getFd());
    removeClient(client->getFd());
};

/*void Server::cleanDisconnectedClients() {
    for (size_t i = 0; i < _clients.size(); ++i) {
        if (_clients[i]->shouldDisconnect()) {
            int fd = _clients[i]->getFd();
            
            // Fermer le socket
            close(fd);
            
            // Supprimer le fd du tableau pollfd
            removeFromPollFds(fd);
            
            // Libérer la mémoire
            delete _clients[i];
            _clients.erase(_clients.begin() + i);
            --i;
        }
    }
}*/