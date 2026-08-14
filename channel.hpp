class Channel
{
    private:
        Server         *_server;
        std::string    _name;
        vector<Client> _Clients;
        vector<Client> _Admins;
        vector<Client> _Invited;
        size_t         _user_limit;
        size_t         _user_size;
        std::string    _topic;
        std::string    _room_password;
        bool           _invite_only;
        bool           _topic_admin_only;
        bool           _room_password_active;
    public:
        Channel(Server, std::string, Client);
        Channel(Server, std::string, std::string, Client);
        ~Channel();
        std::string hash_password(const std::string&);
        void    join(Client);
        void    join(Client, Message);
        void    quit(Client);
        void    editTopic(std::string);
        void    editInvite_only();
        void    editTopic_admin_only();
        void    set_password(std::string);
        void    add_Admin(Client);
        void    editUser_limit();
        void    broadcast(std::string);
        class   ChannelFullException: public std::exception
        {
            const char* what() const throw();
        };
        class   InvalidPasswordException: public std::exception
        {
            const char* what() const throw();
        };
        class   MissingInviteException: public std::exception
        {
            const char* what() const throw();
        };
        class   MissingAuthorizationException: public std::exception
        {
            const char* what() const throw();
        };
        class   MemberNotFoundException: public std::exception
        {
            const char* what() const throw();
        };
}