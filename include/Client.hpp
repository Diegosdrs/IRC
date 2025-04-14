/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:10 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/14 14:01:29 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <memory>
#include <netinet/in.h>
 
class Client
{
    public:
        Client(int socket, std::string nickname);
        void send_message(const std::string &message);
        void receive_message();
        
    private:
        int socket;
        std::string nickname;
        std::string password;
        sockaddr_in address;
        bool isAuthenticated; 
};