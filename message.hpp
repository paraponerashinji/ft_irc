class Message
{
    protected:
        Server      *_server;
        Client      _user;
        std::string _text;
    public:
        Message(Server *server, Client, std::string);
        virtual ~Message();
        virtual void exec(std::string) = 0;
};

class Pass: public Message
{
    public:
        void exec(std::string);
};

class Nick: public Message
{
    public:
        void exec(std::string);
};

class User: public Message
{
    public:
        void exec(std::string);
};

class Privmsg: public Message
{
    public:
        void exec(std::string);
};

class Quit: public Message
{
    public:
        void exec(std::string);
};

class Join: public Message
{
    private:
        _password;
    public:
        std::string getPassword();
        void exec(std::string);
};

class Topic: public Message
{
    public:
        void exec(std::string);
};

class Kick: public Message
{
    public:
        void exec(std::string);
};

class Invite: public Message
{
    public:
        void exec(std::string);
};

class Mode: public Message
{
    public:
        void exec(std::string);
};