#include "server.hpp"
#include "message.hpp"

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
    Channel();
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
        void    add_Invited(Client);
        void    editUser_limit();
        void    broadcast(std::string);
        std::string     getName() const;
        vector<Client>  getClients();
        vector<Client>  getAdmins();
        Client  getAdmins(Client);
        Client  getClients(Client);
        bool    isTopicAdmin();
        class   ChannelFullException: public std::exception
        {
            int errorCode() const;
        };
        class   InvalidPasswordException: public std::exception
        {
            int errorCode() const;();
        };
        class   MissingInviteException: public std::exception
        {
            int errorCode() const;
        };
        class   MissingAuthorizationException: public std::exception
        {
            int errorCode() const;
        };
        class   MemberNotFoundException: public std::exception
        {
            int errorCode() const;
        };
};