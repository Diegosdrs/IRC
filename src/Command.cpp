/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:07:04 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/17 14:18:09 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Command.hpp"
#include "../include/Client.hpp"
#include "../include/Channel.hpp"

Command::Command(){};

Command::~Command(){};

int Command::kick(std::string channel_name, std::string client_to_eject, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    // Si on laisse dans command, faudra verifier si le client 
    // est operator dans la class server
    // if (this->_is_operator == false)
    // {
    //     // ERROR
    //     std::cerr << "You are not the operator" << std::endl;
    //     return (381);
    // }
    if (this->verif_client(client_to_eject, clients) == 1)
    {
        // ERROR
        std::cout << "This client doesn't exist" << std::endl;
        return (1);
    }
    if (this->verif_channel(channel_name, channels) == 1)
    {
        // ERROR
        std::cout << "This channel doesn't exist" << std::endl;
        return (1);
    }
    std::vector<Channel*>::iterator it = channels.begin();
    while(it != channels.end())
    {
        if ((*it)->get_name() == channel_name)
        {
            Client *test = (*it)->get_client(client_to_eject);
            if (test != NULL)
            {
                // (*it)->remove_client(test);
                // test->_channels.erase(it);
                test->leave_channel(channel_name, channels);
                std::cout << client_to_eject << " is ejected from " << channel_name << " channel" << std::endl;
                return (0);
            }
            // ERROR
            std::cout << "This client is not in this channel" << std::endl;
            return (1);
        }
        it++;
    }
    return (0); 
} 

int Command::verif_client(std::string client_to_verif, std::vector<Client*> clients)
{
    std::vector<Client*>::iterator it = clients.begin();
    while(it != clients.end())
    {
        if ((*it)->get_nickname() == client_to_verif || (*it)->get_username() == client_to_verif)
        {
            return (0);
        }
        it++;
    }
    return (1);
}

int Command::verif_channel(std::string channel_to_verif, std::vector<Channel*> channels)
{
    std::vector<Channel*>::iterator it = channels.begin();
    while(it != channels.end())
    {
        if ((*it)->get_name() == channel_to_verif)
        {
            return (0);
        }
        it++;
    }
    return (1);
}