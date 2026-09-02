#include "server.hpp"
#include "./include/client.hpp"
#include <cstdio>
#include <map>

void run_server_loop(Server &server)
{
    std::vector<struct pollfd> fds; // liste des sockets a surveiller
    std::map<int, Client> pending_clients;
    struct pollfd server_pollfd;
    server_pollfd.fd = server.getServerFd();
    server_pollfd.events = POLLIN; // on attend des donnees sur la socket serveur
    server_pollfd.revents = 0;
    fds.push_back(server_pollfd); // on ajoute la socket serveur dans la liste

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

            Client new_client(client_fd); // cree un client en attente de PASS
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
            if (n <= 0)
            {
                close(fds[i].fd); // ferme la socket client
                pending_clients.erase(fds[i].fd);
                server.removeClient(fds[i].fd); // retire le client du serveur
                fds.erase(fds.begin() + i); // retire la socket de la liste poll
                --i;
                continue;
            }

            Client *buffer_client = NULL;
            Client &server_client = server.getClientRef(fds[i].fd);
            std::map<int, Client>::iterator pending_it = pending_clients.find(fds[i].fd);

            if (server_client.getFd() != -1)
                buffer_client = &server_client;
            else if (pending_it != pending_clients.end())
                buffer_client = &pending_it->second;

            if (buffer_client == NULL)
                continue;

            buffer_client->appendToBuffer(std::string(buffer, n)); // ajoute les donnees recues au buffer du client

            while (true)
            {
                Client *client_ptr = NULL;
                bool active_client = false;
                Client &current_server_client = server.getClientRef(fds[i].fd);
                std::map<int, Client>::iterator current_pending_it = pending_clients.find(fds[i].fd);

                if (current_server_client.getFd() != -1)
                {
                    client_ptr = &current_server_client;
                    active_client = true;
                }
                else if (current_pending_it != pending_clients.end())
                    client_ptr = &current_pending_it->second;

                if (client_ptr == NULL)
                    break;

                if (client_ptr->getBuffer().find("\r\n") == std::string::npos)
                    break;

                std::string::size_type pos = client_ptr->getBuffer().find("\r\n");
                std::string line = client_ptr->getBuffer().substr(0, pos); // extrait une ligne complete IRC
                std::string remaining = client_ptr->getBuffer().substr(pos + 2); // garde le reste du buffer
                client_ptr->setBuffer(remaining); // remplace le buffer sans la ligne traitee
                server.receiveMessage(*client_ptr, line); // envoie la commande au serveur pour traitement

                if (!active_client && client_ptr->isRegistered())
                {
                    server.addClient(*client_ptr); // ajoute le client au serveur apres PASS
                    pending_clients.erase(fds[i].fd);
                }

                if (remaining.empty())
                    break;
            }
        }
    }
}