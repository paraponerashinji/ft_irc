Class   Server {
    private:
    vector<Client> Clients
    vector<Channel> Channels
    std::string password;
    int port;
}

class   Client {
    nickname
    socket
    <string>Channel
}

class Channel {
    vector<Client>
}

receive_message(Client, std::string);
PRIVMSG Channel
INVITE

send_message(Client, std::string)