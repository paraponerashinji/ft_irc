class Channel
{
    private:
        vector<Client> Clients;
        vector<Client> Admins;
        size_t         user_limit;
        size_t         user_size;
        std::string    topic;
        std::string    room_password;
        bool           invite_only;
        bool           topic_admin_only;
        bool           room_password_active;
    public:

}