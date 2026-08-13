class Message
{
    private:
        std::string message;
        vector<std::string> message_cut;
        Client      user;
    public:
        Message(Client, std::string);
        virtual ~Message();
        void    message_error();
        Client  get_sender();
        std::string get_message();
        vector<std::string> split_message(std::string);
};

class Private_message: public Message
{
    private:
        Client  target;
        std::string text;
    public:
        Private_message(Client, std::string);
        ~Private_message();
        Client  get_target();
        std::string get_text();
}

class Channel_message: public Message
{
    private:
        Channel channel;
    public:
        Channel_message(Client, Channel, std::string);
        virtual ~Channel_message();
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
        ~Target_command();
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
        ~Mode_command();
        std::string get_flag();
        std::string get_args();
};

class Topic_command: public Channel_message
{
    private:
        std::string args;
    public:
        Topic_command(Client, Channel, std::string);
        ~Topic_command();
        std::string get_args();
};