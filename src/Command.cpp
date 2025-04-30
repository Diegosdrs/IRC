/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:07:04 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/30 14:45:46 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Command.hpp"
#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include <sstream>

Command::Command(){};

Command::~Command(){};

//----------------------------- COMMANDES ------------------------------------


int Command::kick(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    if (this->verif_client(input[2], clients) == 1)
    {
        std::cout << "This client doesn't exist" << std::endl;
        return (1);
    }
    std::vector<Channel*>::iterator it = channels.begin();
    std::string channel_name = input[1];
    channel_name.erase(0,1);
    while(it != channels.end())
    {
        if ((*it)->get_name() == channel_name)
        {
            Client *test = (*it)->get_client(input[2]);
            if (test != NULL)
            {
                test->leave_channel(channel_name, channels);
                if (input.size() > 3)
                    std::cout << input[2] << " is ejected from " << channel_name << " channel because " << input[3] << std::endl;
                else
                    std::cout << input[2] << " is ejected from " << channel_name << " channel" << std::endl;
                return (0);
            }
            std::cout << "This client is not in this channel" << std::endl;
            return (1);
        }
        it++;
    }
    return (1); 
}


int Command::send_message(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    if (input[1][0] == '#' || input[1][0] == '!' || input[1][0] == '&' || input[1][0] == '+')
    {
        std::string channel_name = input[1];
        channel_name.erase(0,1);
        std::vector<Channel*>::iterator it = channels.begin();
        while (it != channels.end())
        {
            if ((*it)->get_name() == channel_name)
            {
                (*it)->send_message(input[2]);
                return (1);
            }
            it++;
        }
        return (1);
    }
    else if (this->verif_client(input[1], clients) == 0)
    {
        std::vector<Client*>::iterator it = clients.begin();
        while (it != clients.end())
        {
            if ((*it)->get_nickname() == input[1])
            {
                //(*it)->receive_message(input[2]);
                return (0); 
            }
            it++;
        }
    }
    std::cerr << "The client " << input[1] << " doesn't exist" << std::endl;
    return (401);
}


int Command::invite(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    if (this->verif_client(input[1], clients) == 1)
    {
        std::cout << "This client doesn't exist" << std::endl;
        return (1);
    }
    std::string channel_name = input[2];
    channel_name.erase(0,1);
    std::vector<Channel*>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            if ((*it)->get_on_invit() == false)
            {
                std::cerr << "Error: This channel doesn't need invitation." << std::endl;
                return (1);
            }
            if ((*it)->get_limit() != -1 && (*it)->get_nbr_of_client() >= (*it)->get_limit())
            {
                std::cerr << "Error: This channel is full." << std::endl;
                return (1);
            }
        }
        it++;
    }
    std::vector<Client*>::iterator ite = clients.begin();
    while(ite != clients.end())
    {
        if (input[1] == (*ite)->get_nickname())
        {
            if((*ite)->is_in_channel(channel_name) == false)
            {
                std::string message_begin = "You have been invited on ";
                std::string message_end = " channel";
                std::string message = message_begin + input[2] + message_end;
                //(*ite)->receive_message(message);
                (*ite)->add_channel_invited(*it);
                return (0);
            }
            std::cerr << "Error : this client is already in this channel" << std::endl;
            return (1);
        }
        ite++;
    }
    return (0);
}

int Command::topic(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    (void)clients;
    std::string channel_name = input[1];
    channel_name.erase(0,1);
    std::vector<Channel*>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            if (input.size() == 2)
            {
                if ((*it)->get_topic() != "default")
                {
                    std::cout << "The topic is " << (*it)->get_topic() << std::endl;
                    return (0); // 332
                }
                std::cerr << "This channel doesn't have topic yet" << std::endl;
                return (331);
            }
            else
            {
                (*it)->set_topic(input[2]);
                std::string message = "The topic of " + channel_name + " is " + input[2];
                (*it)->send_message(message);
                return (0);
            }
        }
        it++;
    }
    return (0);
}

int Command::mode(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels)
{
    (void)clients;
    std::string channel_name = input[1];
    channel_name.erase(0,1);
    std::vector<Channel*>::iterator it = channels.begin();
    
    if (input.size() >= 2)
        input.erase(input.begin(), input.begin() + 2);
    
    if (input[0][0] != '+' && input[0][0] != '-')
    {
        std::cerr << "Error : bad arguments" << std::endl;
        return (1);
    }
    while (it != channels.end())
    {
        if (channel_name == (*it)->get_name())
        {
            break;
        }
        it++;
    }
    
    // KOL --> arguments
    // IT  --> sans arguments
    size_t i = 0;
    size_t j = 0;
    int I = 0;
    int T = 0;
    int K = 0;
    int O = 0;
    int L = 0;
    int I_int = 0;
    int T_int = 0;
    int K_int = 0;
    int O_int = 0;
    int L_int = 0;
    int limit_res = 0;
    while(i < input.size())
    {
        if (input[i][j] == '+')
        {
            while(j < input[i].size())
            {
                if (input[i][j] == 'i')
                {
                    if (I_int == 0)
                        I_int = 1;
                    if (I < 0)
                        I--;
                    else
                        I++;                      
                    I *= -1;
                }
                if (input[i][j] == 't')
                {
                    if (T_int == 0)
                        T_int = 1;
                    if (T < 0)
                        T--;
                    else
                        T++; 
                    T *= -1;
                }
                if (input[i][j] == 'k')
                {
                    if (K_int == 0)
                        K_int = 1;
                    if (i + 1 >= input.size())
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    if (input[i + 1][j] == '+' || input[i + 1][j] == '-')
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    if (!(*it)->get_pass().empty())
                    {
                        std::cerr << "Error : this channel has already a password" << std::endl;
                        return (1);
                    }
                    if (is_valid_password(input[i + 1]) == 1)
                    {
                        std::cerr << "Error : incorrect password definition" << std::endl;
                        return (1);
                    }
                    (*it)->set_pass(input[i + 1]);
                    if (K < 0)
                        K--;
                    else
                        K++; 
                    K *= -1;
                }
                if (input[i][j] == 'o')
                {
                    if (O_int == 0)
                        O_int = 1;
                    if (i + 1 >= input.size())
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    if (input[i + 1][j] == '+' || input[i + 1][j] == '-')
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    is_valid_client(input, i + 1, clients, *it);
                    if (O < 0)
                        O--;
                    else
                        O++; 
                    O *= -1;
                }
                if (input[i][j] == 'l')
                {
                    if (L_int == 0)
                        L_int = 1;
                    if (i + 1 >= input.size())
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    if (input[i + 1][j] == '+' || input[i + 1][j] == '-')
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    limit_res = is_number(input[i + 1]);
                    if (limit_res == 0)
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    if (L < 0)
                        L--;
                    else
                        L++; 
                    L *= -1;
                }
                j++;
            }
            j = 0;
        }
        if (input[i][j] == '-')
        {
            while(j < input[i].size())
            {
                if (input[i][j] == 'i')
                {
                    if (I_int == 0)
                        I_int = 2;
                    if (I < 0)
                        I--;
                    else
                        I++; 
                    I *= -1;
                }
                if (input[i][j] == 't')
                {
                    if (T_int == 0)
                        T = 2;
                    if (T < 0)
                        T--;
                    else
                        T++; 
                    T *= -1;
                }
                if (input[i][j] == 'k')
                {
                    if (K_int == 0)
                        K_int = 2;
                    if (K < 0)
                        K--;
                    else
                        K++; 
                    K *= -1;
                }
                if (input[i][j] == 'o')
                {
                    if (O_int == 0)
                        O_int = 2;
                    if (i + 1 >= input.size())
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    if (input[i + 1][j] == '+' || input[i + 1][j] == '-')
                    {
                        std::cerr << "Error : bad arguments" << std::endl;
                        return (1);
                    }
                    is_valid_client_2(input, i + 1, clients, *it);
                    if (O < 0)
                        O--;
                    else
                        O++; 
                    O *= -1;
                }
                if (input[i][j] == 'l')
                {
                    if (L_int == 0)
                        L_int = 2;
                    if (L < 0)
                        L--;
                    else
                        L++; 
                    L *= -1;
                }
                j++;
            }
            j = 0;
        }
        i++;
    }
    if (I < 0 && I_int == 1)
        (*it)->set_on_invit(true);
    if (I > 0 && I_int == 2)
        (*it)->set_on_invit(false);
    if (T < 0 && T_int == 1)
        (*it)->set_restriction_topic(true);
    if (T > 0 && T_int == 2)
        (*it)->set_restriction_topic(false);
    if (K < 0 && K_int == 2)
        (*it)->set_pass("");
    if (L < 0 && L_int == 1)
        (*it)->set_limit(limit_res);
    if (L > 0 && L_int == 2)
        (*it)->set_limit(-1);
    return (0);
}


//----------------------------- FONCTIONS UTILES ------------------------------------



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

int Command::is_number(std::string nbr)
{
    std::istringstream stream(nbr);
    int number;
    if (stream >> number && stream.eof())
        return number;
    return 0;
}

int Command::is_valid_password(std::string pass)
{
    if (pass.empty())
        return (1);
    if (pass.find(' ') != std::string::npos)
        return (1);
    if (pass.length() > 32)
        return (1);
    return (0);
}

int Command::is_valid_client(std::vector<std::string> input, int index, std::vector<Client*> clients, Channel *channel)
{
    size_t i = index;
    std::string client_to_verif;
    while(i < input.size())
    {
        if (input[i][0] == '+' || input[1][0] == '-')
            return (1);
        client_to_verif = input[i];
        if (channel->get_client(client_to_verif) == NULL)
        {
            std::cerr << "Error : client not in this channel" << std::endl;
            return (1);
        }
        std::vector<Client*>::iterator it = clients.begin();
        while(it != clients.end())
        {
            if ((*it)->get_nickname() == client_to_verif || (*it)->get_username() == client_to_verif)
            {
                if (channel->get_operator(*it) != NULL)
                {
                    std::cerr << "Error : client already operator" << std::endl;
                    return (1);
                }
                channel->add_operator(*it);
                (*it)->add_channel_operator(channel);
                break ; 
            }
            it++;
        }
        i++;
    }
    return (0);
}

int Command::is_valid_client_2(std::vector<std::string> input, int index, std::vector<Client*> clients, Channel *channel)
{
    size_t i = index;
    std::string client_to_verif;
    while(i < input.size())
    {
        if (input[i][0] == '+' || input[1][0] == '-')
            return (1);
        client_to_verif = input[i];
        if (channel->get_client(client_to_verif) == NULL)
        {
            std::cerr << "Error : client not in this channel" << std::endl;
            return (1);
        }
        std::vector<Client*>::iterator it = clients.begin();
        while(it != clients.end())
        {
            if ((*it)->get_nickname() == client_to_verif || (*it)->get_username() == client_to_verif)
            {
                if (channel->get_operator(*it) == NULL)
                {
                    std::cerr << "Error : client not operator" << std::endl;
                    return (1);
                }
                channel->supp_operator(*it);
                (*it)->supp_channel_operator(channel);
                break ; 
            }
            it++;
        }
        i++;
    }
    return (0);
}