/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:26:09 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/17 13:44:02 by dsindres         ###   ########.fr       */
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



class Client;
class Channel;

class Command
{
    public:
        Command();
        ~Command();

        // COMMANDES
        int kick(std::string channel_name, std::string client_to_eject, std::vector<Client*> clients, std::vector<Channel*>channels);
        
        // Fonctions utiles
        int verif_client(std::string client_to_verif, std::vector<Client*> clients);
        int verif_channel(std::string channel_to_verif, std::vector<Channel*> channels);

    private:
};