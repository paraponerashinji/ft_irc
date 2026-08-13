class Message
{
    private:
        std::string message;
        Client      user;
    public:
        Message(Client, std::string);
        Client  get_sender();
        std::string get_message();
};

class Channel_message: public Message
{
    private:
        Channel channel;
    public:
        Channel_message(Client, Channel, std::string);
        Channel get_channel();
};

class Target_command: public Channel_message
{
    private:
        Client  target;
        std::string command;
        std::string args;
    public:
        Target_command(Client, std::string);
        Client  find_target(std::string);
        std::string find_command(std::string);
        std::string find_args(std::string);
        Client  get_target();
        std::string get_command();
        std::string get_args();
};

class Mode_command: public Channel_message
{
    private:
        std::string flag;
        std::string args;
    public:
        Mode_command(Client, Channel, std::string);
        std::string find_flag(std::string);
        std::string find_args(std::string);
        std::string get_flag();
        std::string get_args();
};

class Topic_command: public Channel_message
{
    private:
        std::string args;
    public:
        Topic_command(Client, Channel, std::string);
        std::string find_args(std::string);
        std::string get_args();
};