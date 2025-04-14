/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:35:44 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/14 14:48:37 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

const char  *Server::BadPortException::what() const throw()
{
    return ("bad port");
}

const char  *Server::EmptyPasswordException::what() const throw()
{
    return ("empty password");
}

Server::Server(int port, std::string password)
{
    // peut-etre des verifs sur les ports et throw l'exception
    if (port < 1024 || port > 65535)
        throw BadPortException();
    this->_port = port;
    if (password.empty())
        throw Server::EmptyPasswordException();
    this->_password = password;
    this->start();
}

void Server::start()
{
    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_server_fd < 0)
    {
        perror("Error : probleme creation socket");
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // Accepter toutes les IPs
    addr.sin_port = htons(this->_port);  // Le port du serveur

    // Lier le socket à l'adresse
    if (bind(this->_server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }
    
    // Écouter les connexions
    if (listen(this->_server_fd, 5) < 0) {
        perror("Error listening on socket");
        exit(1);
    }

}
