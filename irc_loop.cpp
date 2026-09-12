#include "./include/server.hpp"
#include "./include/client.hpp"
#include "./include/exception.hpp"
#include <cstdio>
#include <map>

std::vector<std::string> Server::parseCommand(std::string text)
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
}
void Server::executeCommand(Client* client, const std::string& rawLine) {
    try {
        
        std::vector<std::string> tokens = parseCommand(rawLine);
        if (tokens.empty())
            return;

        std::string cmdName = tokens[0];
        for (size_t i = 0; i < cmdName.length(); ++i)
            cmdName[i] = std::toupper(cmdName[i]);

        std::vector<std::string> params(tokens.begin() + 1, tokens.end());

        std::map<std::string, CommandHandler>::iterator it = _commandMap.find(cmdName);

        if (it != _commandMap.end()) {
            CommandHandler handler = it->second;
            (this->*handler)(client, params);
        }
        else {
            throw ERR_UNKNOWNCOMMAND(client, cmdName);
        }
    }
    catch (IrcException &e) {
        sendError(client, e.what());
    }
}
// RUN SERVER LOOP V2
void    Server::run_server_loop()
{
    std::vector<struct pollfd> fds;
    struct pollfd server_pollfd;
    server_pollfd.fd = getServerFd();
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    fds.push_back(server_pollfd);
    std::cout << BGREEN << "Server Up ! Listening to port " << _port << RESET << std::endl;
    while (1)
    {
        int ret = poll(&fds[0], fds.size(), -1);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            perror("poll");
            break;
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (i == 0)
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);

                    int client_fd = accept(getServerFd(), (struct sockaddr*)&client_addr, &client_len); // accepte la connexion d un client
                    std::cout << BBLUE << "New Connection, fd :" << client_fd << RESET << std::endl;
                    if (client_fd < 0)
                    {
                        perror("accept");
                        continue;
                    }
                    char ip[INET_ADDRSTRLEN];
                    std::memset(ip, 0, sizeof(ip));
                    if (inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip)) == NULL)
                        std::snprintf(ip, sizeof(ip), "unknown");
                
                    int flags = fcntl(client_fd, F_GETFL, 0);
                    if (flags < 0 || fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) < 0)
                    {
                        perror("fcntl");
                        close(client_fd);
                        continue;
                    }
                    Client *new_client = new Client(this, client_fd);
                    new_client->setHostname(ip);
                    struct pollfd client_pollfd;
                    client_pollfd.fd = client_fd;
                    client_pollfd.events = POLLIN;
                    client_pollfd.revents = 0;
                    _clients.push_back(new_client);
                    fds.push_back(client_pollfd);
                }
                else
                {
                    char buffer[512];
                    std::memset(buffer, 0, sizeof(buffer));
                    ssize_t n = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                    if (n <= 0)
                    {
                        std::cout << BRED <<"Client disconnection, fd :" << fds[i].fd << RESET << std::endl;
                        close(fds[i].fd);
                        removeClient(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        continue;
                    }
                    Client *client_ptr = getClientPtr(fds[i].fd);
                    if (client_ptr == NULL)
                    {
                        std::cout << RED <<"fd Error, fd :" << fds[i].fd << RESET << std::endl;
                        continue;
                    }
                    client_ptr->appendToBuffer(std::string(buffer, n));
                    while (!client_ptr->getBuffer().empty())
                    {
                        if (client_ptr->getBuffer().find("\r\n") == std::string::npos)
                            break;
                        size_t pos = client_ptr->getBuffer().find("\r\n");
                        std::string line = client_ptr->getBuffer().substr(0, pos);
                        std::string remaining = client_ptr->getBuffer().substr(pos + 2);
                        client_ptr->setBuffer(remaining);
                        std::cout << BLUE << "Client :" << client_ptr->getFd() << " :" << line << RESET << std::endl; 
                        executeCommand(client_ptr, line);
                    }
                }
            }
        }
    }
}

void Server::run_server_loop(Server &server)
{
    std::vector<struct pollfd> fds; // liste des sockets a surveiller
    std::map<int, Client> pending_clients;
    struct pollfd server_pollfd;
    server_pollfd.fd = server.getServerFd();
    server_pollfd.events = POLLIN; // on attend des donnees sur la socket serveur
    server_pollfd.revents = 0;
    fds.push_back(server_pollfd); // on ajoute la socket serveur dans la liste
    std::cout << "SERVER UP" << std::endl;
    while (true)
    {
        int ret = poll(&fds[0], fds.size(), -1); // attend activite reseau sur plusieurs sockets
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            perror("poll");
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);

            int client_fd = accept(server.getServerFd(), (struct sockaddr*)&client_addr, &client_len); // accepte la connexion d un client
            std::cout << "fd client" << client_fd << std::endl;
            if (client_fd < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    continue;
                perror("accept");
                continue;
            }

            char ip[INET_ADDRSTRLEN];
            std::memset(ip, 0, sizeof(ip));
            if (inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip)) == NULL)
                std::snprintf(ip, sizeof(ip), "unknown");

            int flags = fcntl(client_fd, F_GETFL, 0);
            if (flags < 0 || fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) < 0)
            {
                perror("fcntl");
                close(client_fd);
                continue;
            }
            Client new_client(&server, client_fd); // cree un client en attente de PASS
            new_client.setHostname(ip);
            pending_clients[client_fd] = new_client;

            struct pollfd client_pollfd;
            client_pollfd.fd = client_fd; // fd du client
            client_pollfd.events = POLLIN; // on attend ses donnees
            client_pollfd.revents = 0;
            fds.push_back(client_pollfd); // ajout du client dans la liste a surveiller


        }

        for (size_t i = 1; i < fds.size(); ++i)
        {
            if (!(fds[i].revents & POLLIN))
                continue;

            char buffer[512];
            std::memset(buffer, 0, sizeof(buffer));

            ssize_t n = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0); // lit le message du client

            if (n == 0)
            {
                std::cout << ">>> CLIENT DISCONNECTED" << std::endl;
            
                close(fds[i].fd);
                pending_clients.erase(fds[i].fd);
                server.removeClient(fds[i].fd);
                fds.erase(fds.begin() + i);
                --i;
                continue;
            }
            
            if (n < 0)
            {
                std::cout << ">>> recv error: "
                          << strerror(errno)
                          << " errno=" << errno
                          << std::endl;
            
                // temporairement, ne ferme PAS le client pour EAGAIN/EWOULDBLOCK
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    continue;
            
                close(fds[i].fd);
                pending_clients.erase(fds[i].fd);
                server.removeClient(fds[i].fd);
                fds.erase(fds.begin() + i);
                --i;
                continue;
            }
            Client *buffer_client = NULL;
            Client *server_client = server.getClientPtr(fds[i].fd);
            std::map<int, Client>::iterator pending_it = pending_clients.find(fds[i].fd);

            if (server_client && server_client->getFd() != -1)
                buffer_client = server_client;
            else if (pending_it != pending_clients.end())
                buffer_client = &pending_it->second;

            if (buffer_client == NULL)
                continue;
            buffer_client->appendToBuffer(std::string(buffer, n)); // ajoute les donnees recues au buffer du client
            while (true)
            {
                Client *client_ptr = NULL;
                Client *current_server_client = server.getClientPtr(fds[i].fd);
                std::map<int, Client>::iterator current_pending_it = pending_clients.find(fds[i].fd);
                if (current_server_client && current_server_client->getFd() != -1)
                {
                    client_ptr = current_server_client;
                }
                else if (current_pending_it != pending_clients.end())
                {
                    _clients.push_back(&current_pending_it->second);
                    client_ptr = &current_pending_it->second;
                }
                if (client_ptr == NULL)
                {
                    break;
                }

                if (client_ptr->getBuffer().find("\r\n") == std::string::npos)
                {
                    break;
                }
                std::string::size_type pos = client_ptr->getBuffer().find("\r\n");
                std::string line = client_ptr->getBuffer().substr(0, pos); // extrait une ligne complete IRC
                std::string remaining = client_ptr->getBuffer().substr(pos + 2); // garde le reste du buffer
                client_ptr->setBuffer(remaining); // remplace le buffer sans la ligne traitee
                // envoie la commande au serveur pour traitement
                std::cout << client_ptr->getFd() << " :" << line << std::endl; 
                executeCommand(client_ptr, line);
                if (remaining.empty())
                    break;
                //cleanDisconnectedClients();
            }
        }
    }
}