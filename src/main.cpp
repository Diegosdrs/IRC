/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:37:46 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/14 13:19:47 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstdlib>

void    ft_irc(int port, std::string password)
{
    
}

int verif_port(int port)
{
    port += 0;
    return (0);
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Error : bad argument" << std::endl;
        return (1);
    }
    int port = atoi(av[1]);
    if (port < 1024 || verif_port(port) == 1)
    {
        std::cerr << "Error : bad port" << std::endl;
        return (1);
    }
    if (av[2] == NULL)
    {
        std::cerr << "Error : no password" << std::endl;
        return (1);
    }
    std::string password(av[2]);
    ft_irc(port, password);
    return (0);
}
    
