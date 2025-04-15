/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:10 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/15 15:46:28 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <memory>
#include <list>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include "Server.hpp"
#include "Channel.hpp"

class Channel;
class Server;

class Client
{
    public:
        Client();
        Client(int socket);
        Client(Client const &other);
        Client &operator=(Client const &other);
        ~Client();
        
        // Set attributs
        void set_nickname(std::string nickname, std::vector<Client> clients);
        void set_username(std::string usrname, std::vector<Client> clients);

        // Get attributs
        std::string get_nickname();
        std::string get_username();        

        // Gestion de la connexion
        //void connect();
        //void send_message(const std::string &message);
        //void receive_message();

        // Gestion des canaux
        int join_channel(std::string channel_name, std::vector<Channel>channels);
        void leave_channel(std::string channel_name, std::vector<Channel>channels);
        bool is_in_channel(Channel const &chan);
        
    private:
        int _socket;
        std::string _nickname;
        std::string _username;
        sockaddr_in _address;
        bool _isAuthenticated;
        std::vector<Channel> _channels; 
};