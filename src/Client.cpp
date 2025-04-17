/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:12:33 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/17 15:01:11 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"
#include "../include/Command.hpp"
#include "../include/Channel.hpp"

Client::Client(){};

Client::Client(int socket) 
{
    this->_socket = socket;
    this->_nickname = "default";
    this->_username = "default";
    this->_isAuthenticated = false;
    this->_is_operator = false;
    this->_command = new Command();
    // premiere connexion entre client et serveur a faire dans la classe serveur ou client ?
    //this->connect();
    std::cout << "Client join the connexion !" << std::endl;
}

// A voir comment l'appeler dans la fonction generale
Client::~Client()
{
    delete this->_command;
    std::vector<Channel*>::iterator it = this->_channels.begin();
    while (it != this->_channels.end())
    {
        if (*it)
            (*it)->remove_client(this);
        it++;
    }
    this->_channels.clear();
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

int Client::get_socket()
{
    return (this->_socket);
}


void    Client::set_nickname(std::string nickname, std::vector<Client*> clients)
{
    std::vector<Client*>:: iterator it = clients.begin();
    while (it != clients.end())
    {
        if ((*it)->get_nickname() == nickname)
        {
            std::cerr << "Error: nickname already in use" << std::endl;
            return;
        }
        it++;
    }
    this->_nickname = nickname;
}

void    Client::set_username(std::string username, std::vector<Client*> clients)
{
    std::vector<Client*>:: iterator it = clients.begin();
    while (it != clients.end())
    {
        if ((*it)->get_username() == username)
        {
            std::cerr << "Error: username already in use" << std::endl;
            return;
        }
        it++;
    }
    this->_username = username;
}

void    Client::set_operator(bool is_operator)
{
    this->_is_operator = is_operator;
}

//----------------------------- FONCTIONS MEMBRES ------------------------------------


// Nickname DOIT etre set-up sinon segfault
int    Client::join_channel(std::string channel_name, std::vector<Channel*> &channels)
{
    std::vector<Channel*>:: iterator my_it = this->_channels.begin();
    while(my_it != this->_channels.end())
    {
        if ((*my_it)->get_name() == channel_name)
        {
            std::cout << "Client " << this->_nickname << " allready is in " << channel_name << " channel." << std::endl;
            return 0;
        }
        my_it++;
    }
    std::vector<Channel*>:: iterator it = channels.begin();
    while(it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            (*it)->add_client(this);
            this->_channels.push_back(*it);
            std::cout << "Client " << this->_nickname << " add in " << (*it)->get_name() << " channel." << std::endl;
            return 0;
        }
        it++;
    }
    // si join_channel retourne 1, il faut creer un nouveau channel et le mettre dans le 
    // containeur et renvoyer a la fonction et set_operator a true
    return 1;
}

void    Client::leave_channel(std::string channel_name, std::vector<Channel*> &channels)
{
    std::vector<Channel*>:: iterator my_it = this->_channels.begin();
    std::vector<Channel*>:: iterator it = channels.begin();
    while(my_it != this->_channels.end())
    {
        if ((*my_it)->get_name() == channel_name)
        {
            while(it != channels.end())
            {
                if ((*it)->get_name() == channel_name)
                {
                    (*it)->remove_client(this);
                    break ;
                }
                it++;
            }
            this->_channels.erase(my_it);
            std::cout << "Client " << this->_nickname << " has left " << channel_name << " channel." << std::endl;
            return ;
        }
        my_it++;
    }
    std::cout << "Client " << this->_nickname << " is not in " << channel_name << " channel." << std::endl;
    return ;
}

void    Client::leave_channel_from_dest_channel(Channel *channel)
{
    std::vector<Channel*>::iterator it = this->_channels.begin();
    while (it != this->_channels.end())
    {
        if (*it == channel)
        {
            this->_channels.erase(it);
            return ;
        }
        it++;
    }
    // OU juste 
    //this->_channels.erase(channel);
}

bool    Client::is_in_channel(std::string channel_name)
{
    std::vector<Channel*>::iterator it = _channels.begin();
    while(it != this->_channels.end())
    {
        if (channel_name == (*it)->get_name())
            return true;
        it++;
    }
    return false;
}

void    Client::send_message(std::string const &message)
{
    std::string msg = message + "\r\n";
    ssize_t bytes_sent = send(this->_socket, msg.c_str(), msg.length(), 0);
    if (bytes_sent < 0)
        std::cerr << "Error : " << this->_nickname << " doesn't send the message" << std::endl;
        
}

int Client::execute_command(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    std::vector<std::string>::iterator it = input.begin();
    while (it != input.end())
    {
        
        it++;
    }
    
    // if (this->_is_operator == true && channel_name == this->)
    // {
    //     if (command == "KICK")
    //     {
    //         return (this->_command->kick(channel_name, client_to_eject, clients, channels)); 
    //     } 
    // }   
    // else
    // {
    //     std::cerr << "You are not the operator" << std::endl;
    //     return (381);
    // }
    // return (1);
}
