/*void    handlePrivmsg(Client* sender, Channel* channel, const std::string& text) {
    std::string msg = ":" + sender->getPrefix() + " PRIVMSG " + channel->getName() + " :" + text + "\r\n";

    channel->broadcast(sender, msg);
}*/

#include "../include/server.hpp"
#include "../include/client.hpp"
#include "../include/exception.hpp"
#include "../include/channel.hpp"
void Server::Pass(Client* client, const std::vector<std::string>& params) {
    // 1. Vérifier si le client est déjà enregistré (ERR_ALREADYREGISTRED - 462)
    if (client->isRegistered()) {
        throw ERR_ALREADYREGISTRED(":You may not reregister");
        //sendError(client, "462", "ERR_ALREADYREGISTRED", ":Unauthorized command expected. Already registered");
        return;
    }

    // 2. Vérifier si l'argument du mot de passe est présent (ERR_NEEDMOREPARAMS - 461)
    if (params.empty() || params[0].empty()) {
        throw ERR_NEEDMOREPARAMS("<command> :Not enough parameters");
        //sendError(client, "461", "ERR_NEEDMOREPARAMS", "PASS :Not enough parameters");
        return;
    }

    // 3. Vérifier si le mot de passe fourni correspond à celui du serveur (ERR_PASSWDMISMATCH - 464)
    if (params[0] != _password) {
        throw ERR_PASSWDMISMATCH("");
        
        // Optionnel mais recommandé : déconnecter immédiatement le client s'il se trompe de MDP
        //client->setShouldDisconnect(true)
        // TODO : ENLEVER LE NOUVEAU CLIENT DE "PENDING_CLIENTS"
        return;
    }

    // 4. Valider le mot de passe pour ce client
    client->setRegistered(true);
}

bool Server::isValidNickname(const std::string& nick) {
    if (nick.empty() || nick.length() > 9) // RFC fixe la limite à 9 car (souvent assouplie à 18)
        return false;
    
    // Le 1er caractère ne doit pas être un chiffre ou un caractère spécial réservé
    std::string invalidStart = "#$&:0123456789,?!@";
    if (invalidStart.find(nick[0]) != std::string::npos)
        return false;

    // Caractères autorisés : lettres, chiffres, et [ ] \ ` _ ^ { | }
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

    // Parcourt tous les canaux de l'utilisateur
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

void Server::Nick(Client* client, const std::vector<std::string>& params) {
    // 1. Vérification des arguments (ERR_NONICKNAMEGIVEN - 431)
    if (params.empty() || params[0].empty()) {
        throw ERR_NONICKNAMEGIVEN(":No nickname given");
        //sendError(client, "431", "ERR_NONICKNAMEGIVEN", ":No nickname given");
        return;
    }

    std::string newNick = params[0];

    // 2. Validation de la syntaxe du pseudo (ERR_ERRONEUSNICKNAME - 432)
    if (!isValidNickname(newNick)) {
        throw ERR_ERRONEUSNICKNAME("<nick> :Errorneus nickname");
        //sendError(client, "432", "ERR_ERRONEUSNICKNAME", newNick + " :Erroneous nickname");
        return;
    }

    // 3. Vérification de la disponibilité (ERR_NICKNAMEINUSE - 433)
    Client* existingClient = &getClientRef(newNick);
    if (existingClient && existingClient != client) {
        throw ERR_NICKNAMEINUSE("<nick> :Nickname is already in use");
        //sendError(client, "433", "ERR_NICKNAMEINUSE", newNick + " :Nickname is already in use");
        return;
    }

    // 4. Cas N°1 : Changement de pseudo une fois DÉJÀ enregistré
    if (client->isRegistered()) {
        if (existingClient != client) {
            throw ERR_NICKCOLLISION("<nick> :Nickname collision KILL");
            return;
        }
        std::string oldPrefix = client->getPrefix(); // ":old_nick!user@host"
        client->setNickname(newNick);
        
        std::string nickMsg = ":" + oldPrefix + " NICK " + newNick + "\r\n";
        
        // Notifier le client lui-même
        send(client->getFd(), nickMsg.c_str(), nickMsg.length(), MSG_NOSIGNAL);
        
        // Broadcast aux autres membres de tous ses canaux
        broadcastToCommonChannels(client, nickMsg);
    }
    // 5. Cas N°2 : Définition du pseudo pendant L'ENREGISTREMENT
    else {
        client->setNickname(newNick);
        
        // Tenter de finaliser l'enregistrement si PASS et USER sont déjà reçus
        //checkRegistration(client);
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
    // 1. Vérifier si le client est déjà enregistré (ERR_ALREADYREGISTRED - 462)
    if (client->isRegistered()) {
        throw ERR_ALREADYREGISTRED(":You may not reregister");
        //sendError(client, "462", "ERR_ALREADYREGISTRED", ":Unauthorized command expected. Already registered");
        return;
    }

    // 2. Vérifier le nombre d'arguments (ERR_NEEDMOREPARAMS - 461)
    // USER prend 4 paramètres : <username> <hostname> <servername> :<realname>
    if (params.size() < 4) {
        throw ERR_NEEDMOREPARAMS("<command> :Not enough parameters");
        //sendError(client, "461", "ERR_NEEDMOREPARAMS", "USER :Not enough parameters");
        return;
    }

    // 3. Enregistrer le username et le realname
    // (Note: hostname et servername envoyés par le client sont ignorés selon les RFC récentes)
    client->setUsername(params[0]);
    client->setRealname(params[3]);

    // 4. Marquer le flag USER comme reçu et tenter de finaliser l'enregistrement
    //client->setHasUser(true);
    //checkRegistration(client);
}



void Server::Quit(Client* client, const std::vector<std::string>& params) {
    // 1. Déterminer la raison du départ (par défaut "Client Quit", ou le message fourni)
    std::string reason = "Quit: ";
    if (!params.empty() && !params[0].empty()) {
        reason += params[0];
    } else {
        reason += "Client Quit";
    }

    // 2. Préparer le message IRC au format : :nick!user@host QUIT :reason
    std::string quitMsg = ":" + client->getPrefix() + " QUIT :" + reason + "\r\n";

    // 3. Informer tous les utilisateurs qui partagent un canal avec le client déconnecté
    broadcastToCommonChannels(client, quitMsg);

    const std::vector<std::string>& clientChanNames = client->getChannels();
    // 4. Retirer le client de tous ses canaux
    for (size_t i = 0; i < clientChanNames.size(); ++i) {
        Channel* chan = getChannel(clientChanNames[i]);
        if (chan) {
            _channels[i]->quit(client);            
            // Si le canal est vide après son départ, on peut le supprimer
            /*if (_channels[i]->size() == 0) {
                delete _channels[i];
                _channels.erase(_channels.begin() + i);
                --i; // Ajuster l'index après la suppression
            }*/
        }
    }

    // 5. Marquer le client comme "à déconnecter" pour le nettoyer dans la boucle principale
    //client->setShouldDisconnect(true);
}

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