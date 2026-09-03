void    join(Client *sender, std::string text)
{
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        if (splitted[1][0] != '#')
            throw SyntaxErrorException();
        splitted[1].erase(0, 1);
        try
        {
            Channel channel = sender->_server->getChannel(splitted[1]);
        }
        catch (IrcException &e)
        {
            if (e.errorCode() == 403)
            {
                if (!splitted[2])
                {
                    sender->_server->createChannel(sender, splitted[1])
                    return;
                }
                else if (splitted[2][0] != ':')
                    throw SyntaxErrorException();
                splitted[2].erase(0, 1);
                sender->_server->createChannel(sender, splitted[1], splitted[2]);
                return;
            }
            else
                throw e;
        }
        if (!splitted[2])
            channel.join(sender);
        else if (splitted[2][0] != ':')
            throw SyntaxErrorException();
        splitted[2].erase(0, 1);
        channel.join(sender, splitted[2]);
    }
    catch (IrcException &e)
    {
        sender->_server->ircERROR(sender, e.errorCode(), splitted);
    }
};

void    part(Client *sender, std:string text)
{
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        
    }
};

void    privmsg(Client *sender, std:string text)
{

};

void    kick(Client *sender, std:string text)
{

};

void    invite(Client *sender, std:string text)
{

};

void    topic(Client *sender, std:string text)
{

};

void    mode(Client *sender, std:string text)
{

};