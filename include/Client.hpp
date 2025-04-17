/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:10 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/17 14:57:12 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <memory>
#include <list>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>


class Command;
class Channel;
//class Server;

class Client
{
    public:
        Client();
        Client(int socket);
        ~Client();
        
        // Set attributs
        void set_nickname(std::string nickname, std::vector<Client*> clients);
        void set_username(std::string usrname, std::vector<Client*> clients);
        void set_operator(bool is_operator);

        // Get attributs
        std::string get_nickname();
        std::string get_username();
        int get_socket();       

        // Gestion de la connexion
        void send_message(const std::string &message);
        //void receive_message(std::string const &message, std::vector<Client*> clients, std::vector<Channel*>channels);

        // Gestion des canaux
        int join_channel(std::string channel_name, std::vector<Channel*> &channels);
        void leave_channel(std::string channel_name, std::vector<Channel*> &channels);
        bool is_in_channel(std::string channel_name);
        void leave_channel_from_dest_channel(Channel *channel);

        // Commandes Operateurs
        int execute_command(std::vector<std::string> input, std::vector<Client*> clients, std::vector<Channel*>channels);
                
        // Fonctions utiles
        //int verif_client(std::string client_to_verif, std::vector<Client*> clients);
        //int verif_channel(std::string channel_to_verif, std::vector<Channel*> channels);
        //int verif_client_is_in_channel(std::string client_to_find, std::string channel_name);

    private:
        int _socket;
        std::string _nickname;
        std::string _username;
        bool _isAuthenticated;
        bool _is_operator;
        std::vector<Channel*> _channels;
        Command *_command;
};