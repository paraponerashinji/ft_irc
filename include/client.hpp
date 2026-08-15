class client
{
private:
    // socket
    std::string _nickname;
    std::string _username;
public:
    client();
    ~client();
    void    sendMessage(Client, std::string);
};

