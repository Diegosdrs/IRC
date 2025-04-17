/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:46 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/17 13:43:47 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <memory>

class Client;

class Channel
{
    public:
        Channel();
        Channel(std::string name, Client *opera);
        ~Channel();
        
        // Get attributs
        std::string get_name();
        Client *get_operator();
        Client *get_client(std::string client_name);

        // Statut du Channel
        void  status_channel();
        
        void add_client(Client *client);
        void remove_client(Client *client);
        //void send_message(const std::string &message);

    private:
        std::string _name;
        std::vector<Client*> _clients;
        Client *opera;
};