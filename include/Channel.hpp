/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsindres <dsindres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:46 by dsindres          #+#    #+#             */
/*   Updated: 2025/04/14 13:30:26 by dsindres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <memory>

class Channel
{
    public:
        Channel(std::string name);
        void add_client(std::shared_ptr<Client> client);
        void remove_client(std::shared_ptr<Client> client);
        void send_message(const std::string &message);

    private:
        std::string name;
        std::vector<std::shared_ptr<Client>> clients;
};