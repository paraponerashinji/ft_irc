#include "server.hpp"

void run_server_loop(Server &server)
{
    std::vector<struct pollfd> fds; // liste des sockets a surveiller
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

            int flags = fcntl(client_fd, F_GETFL, 0);
            if (flags < 0 || fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) < 0)
            {
                perror("fcntl");
                close(client_fd);
                continue;
            }

            Client new_client(client_fd); // cree un client avec son fd
            server.addClient(new_client); // ajoute le client au serveur

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
                server.removeClient(fds[i].fd); // retire le client du serveur
                fds.erase(fds.begin() + i); // retire la socket de la liste poll
                --i;
                continue;
            }

            Client &client = server.getClientRef(fds[i].fd);
            if (client.getFd() == -1)
            {
                Client new_client(fds[i].fd);
                server.addClient(new_client);
                client = server.getClientRef(fds[i].fd);
            }

            client.appendToBuffer(std::string(buffer, n)); // ajoute les donnees recues au buffer du client

            while (client.getBuffer().find("\r\n") != std::string::npos)
            {
                std::string::size_type pos = client.getBuffer().find("\r\n");
                std::string line = client.getBuffer().substr(0, pos); // extrait une ligne complete IRC
                std::string remaining = client.getBuffer().substr(pos + 2); // garde le reste du buffer
                client.setBuffer(remaining); // remplace le buffer sans la ligne traitee
                server.receiveMessage(client, line); // envoie la commande au serveur pour traitement
            }
        }
    }
}