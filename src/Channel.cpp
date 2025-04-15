/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:25:11 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/15 15:37:58 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(){};

Channel::Channel(std::string name)
{
    this->_name = name;
}

//----------------------------- GETTER et SETTER ------------------------------------

std::string Channel::get_name()
{
    return (this->_name);
}


//----------------------------- METHODES ------------------------------------


void    Channel::add_client(Client const &client)
{
    this->_clients.push_back(client);
}