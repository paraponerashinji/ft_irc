class Server
{
private:
    /*
    socket
    etc
    */
    vector<Client> _clients;
    vector<Channel> _channels;
    std::string     _password;
public:
    server();
    ~server();
    vector<Client*>  getClients();
    Client          *getClients(std::string);
    vector<Channel*> getChannels();
    Channel         *getChannel(std::string);
    void            ircERROR(Client, int);
    void            receiveMessage(Client, std::string);
    void            sendMessage(Client, std::string);
    std::string     getPassword();
};