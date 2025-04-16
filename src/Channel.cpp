/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:25:11 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/16 13:44:05 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(){};

Channel::Channel(std::string name)
{
    this->_name = name;
}

Channel::~Channel()
{
    std::vector<Client*>::iterator it = this->_clients.begin();
    while (it != this->_clients.end())
    {
        if (*it)
            (*it)->leave_channel_from_dest_channel(this);
        it++;
    }
    this->_clients.clear();
}

//----------------------------- GETTER et SETTER ------------------------------------

std::string Channel::get_name()
{
    return (this->_name);
}


//----------------------------- METHODES ------------------------------------

void    Channel::status_channel()
{
    std::vector<Client*>::iterator it = _clients.begin();
    std::cout << " In this channel : " << this->_name << std::endl;
    while(it != _clients.end())
    {
        std::cout << "client = " << (*it)->get_nickname() << std::endl;
        it++;
    }
}

void    Channel::add_client(Client *client)
{
    this->_clients.push_back(client);
}

void    Channel::remove_client(Client *client)
{
    std::vector<Client*>::iterator it = _clients.begin();
    while(it != _clients.end())
    {
        if (*it == client)
        {
            _clients.erase(it);
            std::cout << "Client " << client->get_nickname() << " removed from channel " << this->_name << std::endl;
            return ;
        }
        it++;
    }
}
