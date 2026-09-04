void    join(Client *sender, std::string text)
{
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        ss(splitted[1]);
        std::vector<std::string> channels;
        size_t i = 0;
        while (getline(ss, channels[i], ','))
            i++;
        for (size_t j = 0; j < i; j++)
        {
            if (channels[j][0] != '#' || channels[j][0] != '&')
                throw SyntaxErrorException();
            channels[j].erase(0, 1);
            try
            {
                Channel channel = sender->_server->getChannel(channels[j]);
            }
            catch (IrcException &e)
            {
                if (e.errorCode() == 403)
                {
                    if (!splitted[2])
                    {
                        sender->_server->createChannel(sender, channels[j])
                        continue
                    }
                    else if (splitted[2][0] != ':')
                        throw SyntaxErrorException();
                    splitted[2].erase(0, 1);
                    sender->_server->createChannel(sender, channels[j], splitted[2]);
                    continue;
                }
                else
                    throw e;
            }
            if (!splitted[2])
                channel.join(sender);
            else if (splitted[2][0] != ':')
                throw SyntaxErrorException();
            else
            {
                splitted[2].erase(0, 1);
                channel.join(sender, splitted[2]);
            }
            std::ostringstream output;
            output << "JOIN #" << channel.getName();
            channel.broadcast(sender, output);
        }
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
        ss(splitted[1]);
        std::vector<std::string> channels;
        size_t i = 0;
        while (getline(ss, channels[i], ','))
            i++;
        for (size_t j = 0; j < i; j++)
        {
            if (channels[j][0] != '#' || channels[j][0] != '&')
                throw SyntaxErrorException();
            channels[j].erase(0, 1);
            Channel channel = sender->_server->getChannel(channels[j]);
            if (!splitted[2])
            {
                splitted.push_back("");
                channel.quit(sender, splitted[2]);
            }
            else if (splitted[2][0] != ':')
                throw SyntaxErrorException();
            else
            {
                splitted[2].erase(0, 1);
                channel.quit(sender, splitted[2]);
            }
            std::ostringstream output;
            if (splitted[2].empty())
                output << "PART #" << channel.getName();
            else
                output << "PART #" << channel.getName() << " :" << splitted[2];
            channel.broadcast(sender, output);
        }
    }
    catch (IrcException &e)
    {
        sender->_server->ircERROR(sender, e.errorCode(), splitted);
    }
};

void    privmsg(Client *sender, std:string text)
{
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        ss(splitted[1]);
        std::vector<std::string> target;
        size_t i = 0;
        while (getline(ss, target[i], ','))
            i++;
        for (size_t j = 0; j < i; j++)
        {
            if (target[j][0] == '#' || target[j][0] == '&')
            {
                channels[j].erase(0, 1);
                Channel channel = sender->_server->getChannel(channels[j]);
                channel.getClients(sender)
                if (!splitted[2])
                    throw EmpyMessageException();
                std::ostringstream output;
                std::ostringstream message;
                for (size_t i = 2; i < splitted.size(); i++)
                    message << splitted[i];                
                output << "PRIVMSG #" << channel.getName() << " " << message;
                channel.broadcast(sender, output);
            }
            else
            {
                Client *client = sender->_server->getClient(target[j]);
                if (!splitted[2])
                    throw EmpyMessageException();
                std::ostringstream output;
                std::ostringstream message;
                for (size_t i = 2; i < splitted.size(); i++)
                    message << splitted[i];                
                output << "PRIVMSG " << client->getName() << " " << message;
                sender->sendMessage(client, output);
            }
        }
    }
    catch (IrcException &e)
    {
        sender->_server->ircERROR(sender, e.errorCode(), splitted);
    }
};

void    kick(Client *sender, std:string text)
{
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        if (splitted[1][0] != '#' || splitted[1][0] != '&')
            throw SyntaxErrorException();
        splitted[1].erase(0, 1);
        Channel *channel = sender->_server->getChannel(splitted[1]);
        Client *target = channel->getClientPtr(splitted[2]);
        channel->kick(sender, target);
        std::ostringstream output;
        std::ostringstream message;
        if (splitted.size() == 3)
        {
            output << "KICK #" << channel->getName() << " " << target->getName();
            channel->broadcast(sender, output);
            sender->sendMessage(target, output);
            return ;
        }
        for (size_t i = 3; i < splitted.size(); i++)
            message << splitted[i];
        output << "KICK #" << channel->getName() << " " << target->getName() << " " << message;
        channel->broadcast(sender, output);
        sender->sendMessage(target, output);
    }
    catch (IrcException &e)
    {
        sender->_server->ircERROR(sender, e.errorCode(), splitted);
    }
};

void    invite(Client *sender, std:string text)
{
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        if (splitted[1][0] == '#' || splitted[1][0] == '&')
            throw SyntaxErrorException();
        splitted.erase(0,1);
        if (splitted[2][0] != '#' || splitted[2][0] != '&')
            throw SyntaxErrorException();
        splitted.erase(0,1);
        Client *target = channel->getClientPtr(splitted[1]);
        Channel *channel = sender->_server->getChannel(splitted[2]);
        channel->add_Invited(sender, target);
        std::ostringstream output;
        output << "INVITE " << target->getName() << " #" << channel->getName();
        sender->sendMessage(target, output);
    }
    catch (IrcException &e)
    {
        sender->_server->ircERROR(sender, e.errorCode(), splitted);
    }
};

void    topic(Client *sender, std:string text)
{
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        if (splitted[1][0] != '#' || splitted[1][0] != '&')
            throw SyntaxErrorException();
        splitted[1].erase(0, 1);
        Channel *channel = sender->_server->getChannel(splitted[1]);
        std::ostringstream message;
        if (!splitted[2])
            throw EmpyMessageException();
        if (splitted[2][0] !=  ':')
            throw SyntaxErrorException();
        splitted[2].erase(0, 1);
        for (size_t i = 2; i < splitted.size(); i++)
            message << splitted[i];
        channel->editTopic(sender, message);
        std::ostringstream output;
        output << "TOPIC #" << channel->getName() << " :" << message;
        channel->broadcast(sender, output);
    }
    catch (IrcException &e)
    {
        sender->_server->ircERROR(sender, e.errorCode(), splitted);
    }
};

void    mode(Client *sender, std:string text)
{
    bool    make = false;
    std::stringstream ss(text);
    std::vector<std::string> splitted;
    std::string word;
    while (ss >> word)
        splitted.push_back(word);
    try
    {
        if (splitted[1][0] != '#' || splitted[1][0] != '&')
            throw SyntaxErrorException();
        splitted[1].erase(0, 1);
        Channel *channel = sender->_server->getChannel(splitted[1]);
        if (splitted[2][0] != '+' || splitted[2][0] != '-')
            throw SyntaxErrorException();
        if (splitted[2][0] == '+')
            make = true;
        splitted[2].erase(0,1);
        if (splitted[3])
        {
            if (splitted[3][0] != ':')
                throw SyntaxErrorException();
            splitted[3].erase(0, 1);
        }
        size_t k = 3;
        for (size_t i = 0; i < splitted[2].size(); i++)
        {
            char mode[5] = { 'o', 't', 'i', 'k', 'l'};
            size_t j = 0;
            while (j < 5)
            {
                if splitted[2][i] == mode[j];
                    break;
            }
            switch (j)
            {
                case 0:
                    if (!splitted[k])
                        throw MissingArgsException();
                    if (make)
                        channel->add_Admin(sender, splitted[k]);
                    else
                        channel->remove_Admin(sender, splitted[k]);  
                    k++;
                    break;
                case 1:
                    channel->editTopic_admin_only(sender, make);
                    break;
                case 2:
                    channel->editInvite_only(sender, make);
                case 3:
                    if (!make)
                        channel->remove_password();
                    else
                    {
                        if (!splitted[k])
                            throw MissingArgsException();
                        channel->set_password(sender, splitted[k]);
                        k++;
                    }
                    break;
                case 4:
                    if (!splitted[k] || !make)
                        channel->editUser_limit(sender, make);
                    else if (splitted[k].find_first_not_of("0123456789") != std::string::npos)
                    {
                        channel->editUser_limit(sender, std::stoi(splitted[k]));
                        k++;
                    }
                    else
                        channel->editUser_limit(sender);
                    break;
            }
        }
    }
    catch (IrcException &e)
    {
        sender->_server->ircERROR(sender, e.errorCode(), splitted);
    }
};