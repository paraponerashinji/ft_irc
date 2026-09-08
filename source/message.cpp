/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 12:26:19 by aharder           #+#    #+#             */
/*   Updated: 2026/09/08 13:54:37 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"
#include "client.hpp"
#include "server.hpp"
#include "channel.hpp"
#include "exception.hpp"

Message::Message(Server *server, Client *user, std::string text): _server(server), _user(user), _text(text)
{
};

Message::~Message()
{
};

std::string Join::getPassword()
{
    return _password;
};
