#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "exception.hpp"
#include <iostream>
#include <cassert>
#include <sstream>
/*
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
*/

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cout << "./IRC <port> <password>" << std::endl;
        return 1;
    }
    int _port = std::atoi(av[1]);
    int server_fd = create_server_socket(_port); // creer la socket serveur
    if (server_fd < 0)
        return 1;
    Server server(std::string(av[2]), server_fd); // serveur IRC avec mot de passe et fd
    server.setPort(_port);
    server.run_server_loop();
    close(server_fd); // ferme la socket du serveur si on sort de la boucle
    return 0;
}