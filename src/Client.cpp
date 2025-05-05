/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:12:33 by dsindres          #+#    #+#             */
/*   Updated: 2025/05/05 11:01:50 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"
#include "../include/Command.hpp"
#include "../include/Channel.hpp"
#include <algorithm> 
#include <cerrno>


Client::Client(){};

Client::Client(int socket) 
{
    this->_socket = socket;
    this->_nickname = "default";
    this->_username = "default";
    this->_realname = "default";
    this->_is_authenticated = false;
    this->_is_operator = false;
    this->_command = new Command();
}


Client::~Client()
{
    delete this->_command;
    std::vector<Channel*>::iterator ite = this->_operator_channels.begin();
    while (ite != this->_operator_channels.end())
    {
        if (*ite)
            (*ite)->set_operator(this);
        ite++;
    }
    std::vector<Channel*>::iterator it = this->_channels.begin();
    while (it != this->_channels.end())
    {
        if (*it)
            (*it)->remove_client(this);
        it++;
    }
    this->_invited_channels.clear();
    this->_channels.clear();
    this->_operator_channels.clear();
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

int Client::is_operator(std::string channel_name)
{
    std::vector<Channel*>:: iterator it = this->_operator_channels.begin();
    while(it != this->_operator_channels.end())
    {
        if ((*it)->get_name() == channel_name)
        {
            if ((*it)->get_operator(this) != NULL)
                return (0);
            return (1);
        }
        it++;
    }
    return (1);
}


int    Client::set_nickname(std::string nickname, std::vector<Client*> clients, std::vector<Channel*> channels)
{
    (void)channels;
    std::vector<Client*>:: iterator it = clients.begin();
    if (nickname == "default")
    {
        //std::cerr << "Error : you can't take this nickname" << std::endl;
        return (1);
    }
    while (it != clients.end())
    {
        if ((*it)->get_nickname() == nickname)
        {
            //std::cerr << "Error: nickname already in use" << std::endl;
            return (433);
        }
        it++;
    }
    std::string old_nick = this->_nickname;
    this->_nickname = nickname;
    if(old_nick == "default")
        return (0);
    std::string message;
    
    if (this->_username == "default")
        message = ":" + old_nick + "!~ *" + "@localhost NICK :" + this->_nickname;
    if (this->_username != "default")
        message = ":" + old_nick + "!~" + this->_username + "@localhost NICK :" + this->_nickname;
    this->send_message(message, clients, channels);
    return (0);
}

int    Client::set_username(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    (void)channels;
    std::vector<Client*>:: iterator it = clients.begin();
    if (this->_username != "default" && this->_is_authenticated == true)
    {
        //std::cerr << "Error : you already have an username" << std::endl;
        return (462);
    }
    if (input[1] == "default")
    {
        //std::cerr << "Error : you can't take this username" << std::endl;
        return (1);
    }
    while (it != clients.end())
    {
        if ((*it)->get_username() == input[1])
        {
            //std::cerr << "Error: username already in use" << std::endl;
            return (433);
        }
        it++;
    }
    this->_username = input[1];
    this->_realname = input[4];
    return (0);
}

void    Client::set_operator(bool is_operator)
{
    this->_is_operator = is_operator;
}

void    Client::set_autentification(bool reponse)
{
    this->_is_authenticated = reponse;
}


//----------------------------- GESTION DES CHANNELS ------------------------------------


int    Client::join_channel(std::vector<std::string> input, std::vector<Channel*> &channels)
{
    std::string channel_name = input[1];
    channel_name.erase(0, 1);
    std::vector<Channel*>:: iterator my_it = this->_channels.begin();
    if (this->_command->verif_channel(channel_name, channels) == 1)
        return (403);
    while(my_it != this->_channels.end())
    {
        if ((*my_it)->get_name() == channel_name)
        {
            //std::cout << "Client " << this->_nickname << " already is in " << channel_name << " channel." << std::endl;
            return (0);
        }
        my_it++;
    }
    std::vector<Channel*>:: iterator it = channels.begin();
    while(it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            if (input.size() > 2)
            {
                if ((*it)->get_on_invit() == true && this->get_invited_by(*it) == false)
                {
                    //std::cerr << "Error : you haven't been invited" << std::endl;
                    return (473);
                }
                if (input[2] != (*it)->get_pass() && (*it)->get_pass() != "")
                {
                    //std::cerr << "Error : it is not the right password" << std::endl;
                    return (475);
                }
                if ((*it)->get_limit() != -1 && (*it)->get_nbr_of_client() >= (*it)->get_limit())
                {
                    //std::cerr << "Channel " << channel_name << " is full" << std::endl;
                    return (471);
                }
                (*it)->add_client(this);
                this->_channels.push_back(*it);
                if ((*it)->get_on_invit() == true)
                {
                    std::vector<Channel*>::iterator invited_it = std::find(this->_invited_channels.begin(), this->_invited_channels.end(), *it);
                    if (invited_it != this->_invited_channels.end())
                    {
                        this->_invited_channels.erase(invited_it);
                    }
                }             
                //std::cout << "Client " << this->_nickname << " add in " << (*it)->get_name() << " channel." << std::endl;
                std::string message;
                message = ":" + this->_nickname + "!~" + this->_username+ "@localhost JOIN :#" + (*it)->get_name();
                (*it)->send_message(message);
                this->join_message(*it);
                return (0);
            }
            else
            {
                if ((*it)->get_on_invit() == true && this->get_invited_by(*it) == false)
                {
                    //std::cerr << "Error : you haven't been invited" << std::endl;
                    return (473);
                }
                if ((*it)->get_pass() != "")
                {
                    //std::cerr << "Error : you need a password" << std::endl;
                    return (475);
                }
                if ((*it)->get_limit() != -1 && (*it)->get_nbr_of_client() >= (*it)->get_limit())
                {
                    //std::cerr << "Channel " << channel_name << " is full" << std::endl;
                    return (471);
                }
                (*it)->add_client(this);
                this->_channels.push_back(*it);
                if ((*it)->get_on_invit() == true)
                {
                    std::vector<Channel*>::iterator invited_it = std::find(this->_invited_channels.begin(), this->_invited_channels.end(), *it);
                    if (invited_it != this->_invited_channels.end())
                    {
                        this->_invited_channels.erase(invited_it);
                    }
                }
                //std::cout << "Client " << this->_nickname << " add in " << (*it)->get_name() << " channel." << std::endl;
                std::string message;
                message = ":" + this->_nickname + "!~" + this->_username+ "@localhost JOIN :#" + (*it)->get_name();
                (*it)->send_message(message);
                this->join_message(*it);
                return (0);
            }
        }
        it++;
    }
    return (11);
}

void    Client::leave_channel(std::string channel_name, std::vector<Channel*> &channels)
{
    std::vector<Channel*>:: iterator it = channels.begin();
    while(it != channels.end())
    {
        if ((*it)->get_name() == channel_name)
        {
            (*it)->remove_client(this);
            break ;
        }
        it++;
    }
    std::vector<Channel*>::iterator ite = this->_channels.begin();
    while(ite != _channels.end())
    {
        if ((*ite)->get_name() == channel_name)
        {
            this->_channels.erase(ite);
            break ;
        }
        ite++;
    }
    std::cout << "Client " << this->_nickname << " has left " << channel_name << " channel." << std::endl;
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


//----------------------------- COMMANDES------------------------------------



int Client::execute_command(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    if (input[0] == "NICK")
    {
       int res = this->set_nickname(input[1], clients, channels);
       return (res);
    }
    if (input[0] == "USER")
    {
       int res = this->set_username(input, clients, channels);
       return (res);
    }
    if (this->_is_authenticated == false)
    {
        std::cout << "Client not authentificated" << std::endl; 
        return (1);
    }
    if (input[0] == "JOIN")
    {
        int res = this->join_channel(input, channels);
        return (res);
    }
    if (input[0] == "PRIVMSG")
    {
        int res = this->privmsg(input, clients, channels);
        return (res);
    }
    if (input[0] == "KICK")
    {
        int res = this->kick(input, clients, channels);
        return (res);
    }
    if (input[0] == "INVITE") // input.size() == 3
    {
        int res = this->invite(input, clients, channels);
        return (res);
    }
    if (input[0] == "TOPIC") // input.size() 2 ou 3 max
    {
        int res = this->topic(input, clients, channels);
        return (res);
    }
    if (input[0] == "MODE") // input.size() > 2 A FINIIIIIIR !
    {
        int res = this->mode(input, clients, channels);
        return (res);
    }
    return (0);
}

int Client::privmsg(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    if (input[1][0] == '#' || input[1][0] == '!' || input[1][0] == '&' || input[1][0] == '+')
    {
        std::string channel_name = input[1];
        channel_name.erase(0,1);
        if (this->_command->verif_channel(channel_name, channels) == 0)
        {
            if (this->is_in_channel(channel_name) == false)
            {
                //std::cerr << "The client " << _nickname << " is not in " << channel_name << " channel" << std::endl;
                return (404);
            }
            int res = this->_command->send_message(input, clients, channels, this);
            return (res);
        }
        //std::cerr << "The channel " << channel_name << " doesn't exist" << std::endl;
        return (403);
    }
    int res = this->_command->send_message(input, clients, channels, this);
    return (res);
}

int Client::kick(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    std::string channel_name = input[1];
    channel_name.erase(0,1);
    if (this->_command->verif_channel(channel_name, channels) == 1)
    {
        //std::cerr << "Channel " << channel_name << " doesn't exist" << std::endl;
        return (403);
    }
    std::vector<Channel*>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            if ((*it)->get_operator_bool() == false)
            {
                //std::cerr << "Error: No operator in the channel. You cannot use operator commands." << std::endl;
                return (482);
            }
        }
        it++;
    }
    if (this->_is_operator == false || this->is_operator(channel_name) == 1)
    {
        //std::cerr << "You are not the operator of " << channel_name << " channel" << std::endl;
        return (482);
    }
    int res = this->_command->kick(input, clients, channels, this);
    return (res);
}

int Client::invite(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    std::string channel_name = input[2];
    channel_name.erase(0,1);
    if (this->_command->verif_channel(channel_name, channels) == 1)
    {
        //std::cerr << "Channel " << channel_name << " doesn't exist" << std::endl;
        return (403);
    }
    std::vector<Channel*>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            if ((*it)->get_operator_bool() == false)
            {
                //std::cerr << "Error: No operator in the channel. You cannot use operator commands." << std::endl;
                return (482);
            }
        }
        it++;
    }
    if ((this->_is_operator == false || this->is_operator(channel_name) == 1) && (*it)->get_restriction_topic() == true)
    {
        //std::cerr << "You are not the operator of " << channel_name << " channel" << std::endl;
        return (482);
    }
    int res = this->_command->invite(input, clients, channels, this);
    return (res);
}

int Client::topic(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    std::string channel_name = input[1];
    channel_name.erase(0,1);
    if (this->_command->verif_channel(channel_name, channels) == 1)
    {
        //std::cerr << "Channel " << channel_name << " doesn't exist" << std::endl;
        return (403);
    }
    std::vector<Channel*>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            if ((*it)->get_operator_bool() == false && (*it)->get_restriction_topic() == true)
            {
                //std::cerr << "Error: No operator in the channel. You cannot use operator commands." << std::endl;
                return (482);
            }
        }
        it++;
    }
    if (this->_is_operator == false || this->is_operator(channel_name) == 1)
    {
        //std::cerr << "You are not the operator of " << channel_name << " channel" << std::endl;
        return (482);
    }
    int res = this->_command->topic(input, clients, channels, this);
    return (res);
}

int Client::mode(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    std::string channel_name = input[1];
    channel_name.erase(0,1);
    if (this->_command->verif_channel(channel_name, channels) == 1)
    {
        //std::cerr << "Channel " << channel_name << " doesn't exist" << std::endl;
        return (403);
    }
    std::vector<Channel*>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            if ((*it)->get_operator_bool() == false)
            {
                //std::cerr << "Error: No operator in the channel. You cannot use operator commands." << std::endl;
                return (482);
            }
        }
        it++;
    }
    if (this->_is_operator == false || this->is_operator(channel_name) == 1)
    {
        //std::cerr << "You are not the operator of " << channel_name << " channel" << std::endl;
        return (482);
    }
    int res = this->_command->mode(input, clients, channels, this);
    return (res);
}

//----------------------------- FONCTIONS UTILES ------------------------------------


bool Client::get_invited_by(Channel *channel)
{
    std::vector<Channel*>::iterator it = _invited_channels.begin();
    while(it != _invited_channels.end())
    {
        if ((*it) == channel)
            return (true);
        it++;
    }
    return (false);
}

void    Client::add_channel_operator(Channel *channel)
{
    this->_operator_channels.push_back(channel);
}

void    Client::add_channel_invited(Channel *channel)
{
    this->_invited_channels.push_back(channel);
}

void    Client::receive_message(std::string const &message, int socket)
{
    std::string full_message = message + "\r\n";
    int bytes = send(socket, message.c_str(), message.length(), 0);
    if (bytes > 0)
        throw std::runtime_error(std::string("send: ") + std::strerror(errno));
}


void    Client::supp_channel_operator(Channel *channel)
{
    std::vector<Channel*>::iterator it = this->_operator_channels.begin();
    while (it != this->_operator_channels.end())
    {
        if ((*it) == channel)
        {
            this->_operator_channels.erase(it);
            return ;
        }
        it++;
    }
}

void Client::send_message(std::string message, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    (void)channels;
    std::vector<Client*> buffer_client;
    std::vector<Channel*>::iterator it = this->_channels.begin();
    while (it != this->_channels.end())
    {
        std::vector<Client*>::iterator ite = clients.begin();
        while (ite != clients.end())
        {
            if((*it)->get_client((*ite)->get_nickname()) != NULL && std::find(buffer_client.begin(), buffer_client.end(), *ite) == buffer_client.end())
            {
                buffer_client.push_back(*ite);
            }
            ite++;
        }
        it++;
    }
    for (size_t i = 0; i < buffer_client.size(); ++i)
        buffer_client[i]->receive_message(message, buffer_client[i]->get_socket());
}

void Client::join_message(Channel *channel)
{
    if (channel->get_topic() != "default")
    {
        std::string message1;
        message1 = "IRC 332 " + this->_nickname + " #" + channel->get_name() + " :" + channel->get_topic();
        this->receive_message(message1, this->_socket);
    }
    std::string join = channel->join_message();
    std::string message2;
    message2 = "IRC 353 " + this->_nickname + " = #" + channel->get_name() + " :" + join;
    this->receive_message(message2, this->_socket);
    std::string message3;
    message3 = "IRC 366 " + this->_nickname + " #" + channel->get_name() + " :" + "End of /NAMES list.";
    this->receive_message(message3, this->_socket);
}

//----------------------------- DEBUG ------------------------------------


void Client::get_operator()
{
    std::vector<Channel*>::iterator it = _operator_channels.begin();
    while(it != _operator_channels.end())
    {
        std::cout << "    operator --> " << (*it)->get_name() << std::endl;
        it++;
    }
}

void Client::get_channel()
{
    std::vector<Channel*>::iterator it = _channels.begin();
    while(it != _channels.end())
    {
        std::cout << "            --> " << (*it)->get_name() << std::endl;
        it++;
    }
}

void Client::get_invitation()
{
    std::vector<Channel*>::iterator it = _channels.begin();
    while(it != _channels.end())
    {
        std::cout << " invitation --> " << (*it)->get_name() << std::endl;
        it++;
    }
}