#include "server.hpp"

int main(int ac, char **av) {

    if (ac != 3)
        return 1;

    int port = std::atoi(av[1]);
    int server_fd = create_server_socket(port);
    if (server_fd < 0)
        return 1;

    Server server(std::string(av[2]), server_fd);

    std::vector<struct pollfd> fds;
    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    fds.push_back(server_pollfd);

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

            int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len); // accepte la connexion d un client, client_fd = unique a ce client
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

            Client new_client(client_fd);
            server.addClient(new_client);

            struct pollfd client_pollfd;
            client_pollfd.fd = client_fd;
            client_pollfd.events = POLLIN;
            client_pollfd.revents = 0;
            fds.push_back(client_pollfd);

            std::cout << "Nouveau client connecte, fd = " << client_fd << std::endl;
        }

        for (size_t i = 1; i < fds.size(); ++i)
        {
            if (fds[i].revents & POLLIN)
            {
                char buffer[512];
                std::memset(buffer, 0, sizeof(buffer));

                ssize_t n = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0); // lit le message du client
                if (n <= 0)
                {
                    std::cout << "Client deconnecte, fd = " << fds[i].fd << std::endl;
                    close(fds[i].fd);
                    server.removeClient(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    --i;
                    continue;
                }

                std::string message(buffer, n);
                Client client = server.getClient(fds[i].fd);
                if (client.getFd() == -1)
                {
                    Client new_client(fds[i].fd);
                    server.addClient(new_client);
                    client = server.getClient(fds[i].fd);
                }

                server.receiveMessage(client, message);
            }
        }
    }

    close(server_fd);
    return 0;
}