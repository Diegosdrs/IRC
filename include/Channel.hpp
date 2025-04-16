/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:46 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/16 13:40:47 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <string>
#include <memory>
//#include "Server.hpp"
#include "Client.hpp"

class Client;

class Channel
{
    public:
        Channel();
        Channel(std::string name);
        ~Channel();
        
        // Get attributs
        std::string get_name();

        // Statut du Channel
        void  status_channel();
        
        void add_client(Client *client);
        void remove_client(Client *client);
        //void send_message(const std::string &message);

    private:
        std::string _name;
        std::vector<Client*> _clients;
};