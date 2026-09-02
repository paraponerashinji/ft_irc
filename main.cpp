#include "server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return 1;

    int port = std::atoi(av[1]);
    int server_fd = create_server_socket(port); // creer la socket serveur
    if (server_fd < 0)
        return 1;

    Server server(std::string(av[2]), server_fd); // serveur IRC avec mot de passe et fd
    run_server_loop(server);

    close(server_fd); // ferme la socket du serveur si on sort de la boucle
    return 0;
}