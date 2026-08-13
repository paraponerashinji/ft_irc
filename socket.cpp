#include "server.hpp"

int create_server_socket(int port) {

    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // creer la socket serveur (le FD reseau)
    if (server_fd < 0) {
        std::cerr << "socket a echoue" << std::endl;
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { // "Si je relance le serveur, le port reutilisable direct"
        std::cerr << "setsockopt() failed" << std::endl;
        close(server_fd);
        return -1;
    }

    int flags = fcntl(server_fd, F_GETFL, 0); // = mode non bloquant
    if (flags < 0 || fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        std::cerr << "fcntl() failed" << std::endl;
        close(server_fd);
        return -1;
    }

    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);         // Port converti en Big Endian

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { // associe la socket au port
        std::cerr << "bind() failed" << std::endl;
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) { // met le serveur en attente de connexion
        std::cerr << "listen() failed" << std::endl;
        close(server_fd);
        return -1;
    }

    return server_fd;
}