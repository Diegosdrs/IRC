/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:46 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/15 15:24:59 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <memory>
#include "Client.hpp"

class Client;

class Channel
{
    public:
        Channel();
        Channel(std::string name);
        Channel(Channel const &other);
        Channel &operator=(Channel const &other);
        ~Channel();
        
        // Get attributs
        std::string get_name();
        
        
        void add_client(Client const &client);
        void remove_client(std::shared_ptr<Client> client);
        void send_message(const std::string &message);

    private:
        std::string _name;
        std::vector<Client> _clients;
};