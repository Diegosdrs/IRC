/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:37:46 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/17 14:24:20 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "../include/Client.hpp"
#include "../include/Channel.hpp"

int main() {
    // Conteneurs globaux simulant ceux du Server
    std::vector<Client*> clients;
    std::vector<Channel*> channels;

    // Création de clients
    Client* alice = new Client(1);
    Client* bob = new Client(2);
    Client* charlie = new Client(3);

    clients.push_back(alice);
    clients.push_back(bob);
    clients.push_back(charlie);

    // Set nicknames et usernames (avec vérifications)
    alice->set_nickname("Alice", clients);
    alice->set_username("alice42", clients);

    bob->set_nickname("Bob", clients);
    bob->set_username("bob_dev", clients);

    charlie->set_nickname("Charlie", clients);
    charlie->set_username("charlie_lol", clients);

    // Test de nickname déjà pris
    Client* hacker = new Client(4);
    hacker->set_nickname("Alice", clients); // doit afficher une erreur
    delete hacker;

    // Création de channels
    Channel* general = new Channel("general", alice);
    Channel* random = new Channel("random", bob);
    Channel* cpp = new Channel("cpp", charlie);

    alice->set_operator(true);
    bob->set_operator(true);
    charlie->set_operator(true);

    channels.push_back(general);
    channels.push_back(random);
    channels.push_back(cpp);

    // Clients rejoignent les channels
    alice->join_channel("general", channels);
    alice->join_channel("cpp", channels);

    bob->join_channel("general", channels);
    bob->join_channel("random", channels);

    charlie->join_channel("random", channels);
    charlie->join_channel("cpp", channels);


    std::cout << alice->execute_command("KICK", "random", "Bob", clients, channels) << std::endl;
    

    std::cout << "\n--- Suppression manuelle des Channels ---" << std::endl;
    delete general;
    delete random;
    delete cpp;

    std::cout << "\n--- Suppression manuelle des autres clients ---" << std::endl;
    delete alice;
    delete charlie;
    delete bob;

    return 0;
}
















    
    // Clients rejoignent les channels
//     alice->join_channel("general", channels);  // channel existe déjà
//     alice->join_channel("cpp", channels);

//     bob->join_channel("general", channels);
//     bob->join_channel("random", channels);

//     charlie->join_channel("random", channels);
//     charlie->join_channel("cpp", channels);

//     general->status_channel();
//     random->status_channel();
//     cpp->status_channel();

//     // Clients quittent des channels
//     bob->leave_channel("general", channels);
//     charlie->leave_channel("cpp", channels);

//     // Test de départ d'un channel non rejoint
//     alice->leave_channel("random", channels);

//     general->status_channel();
//     random->status_channel();
//     cpp->status_channel();

//     // Test de destruction automatique (déconnexion)
//     std::cout << "\n--- Suppression manuelle de Bob ---" << std::endl;
//     delete bob;
//     random->status_channel();

//     std::cout << "\n--- Suppression manuelle des Channels ---" << std::endl;
//     delete general;
//     delete random;
//     delete cpp;

//     std::cout << "\n--- Suppression manuelle des autres clients ---" << std::endl;
//     delete alice;
//     delete charlie;

//     return 0;
// }
    
