/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:12:33 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/15 15:48:21 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(){};

Client::Client(int socket) 
{
    this->_socket = socket;
    this->_nickname = "default";
    this->_username = "default";
    this->_isAuthenticated = false;
    // premiere connexion entre client et serveur a faire dans la classe serveur ou client ?
    //this->connect();
    std::cout << "Client join the connexion !" << std::endl;
}

Client::~Client()
{
    
}

//----------------------------- GETTER et SETTER ------------------------------------

std::string Client::get_nickname()
{
    return (this->_nickname);
}

std::string Client::get_username()
{
    return (this->_username);
}


void    Client::set_nickname(std::string nickname, std::vector<Client> clients)
{
    std::vector<Client>:: iterator it = clients.begin();
    while (it != clients.end())
    {
        if (it->get_nickname() == nickname)
        {
            std::cerr << "Error: nickname already in use" << std::endl;
            return;
        }
        it++;
    }
    this->_nickname = nickname;
}

void    Client::set_username(std::string username, std::vector<Client> clients)
{
    std::vector<Client>:: iterator it = clients.begin();
    while (it != clients.end())
    {
        if (it->get_username() == username)
        {
            std::cerr << "Error: username already in use" << std::endl;
            return;
        }
        it++;
    }
    this->_username = username;
}

// Nickname DOIT etre set-up sinon segfault
int    Client::join_channel(std::string channel_name, std::vector<Channel>channels)
{
    std::vector<Channel>:: iterator it = channels.begin();
    std::vector<Channel>:: iterator my_it = this->_channels.begin();
    while(it != channels.end())
    {
        while(my_it != this->_channels.end())
        {
            if (my_it->get_name() == it->get_name())
            {
                std::cout << "Client " << this->_nickname << " allready is in " << it->get_name() << " channel." << std::endl;
                return 0;
            }
            my_it++;
        }
        it++;
    }
    it = channels.begin();
    while(it != channels.end())
    {
        if (channel_name == it->get_name())
        {
            it->add_client(this);
            this->_channels.push_back(it);
            std::cout << "Client " << this->_nickname << " add in " << it->get_name() << " channel." << std::endl;
            return 0;
        }
        it++;
    }
    // si join_channel retourne 1, il faut creer un nouveau channel et le mettre dans le 
    // containeur et renvoyer a la fonction et c good
    return 1;
}

void    Client::leave_channel(std::string channel_name, std::vector<Channel>channels)
{
    
}


    
