#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include "Client.h"

class Server
{
public:
    Server(unsigned short port = 54000);
    void Run(); 

private:
    void AcceptNewConnection();
    void ReceiveData(Client* client);
    void RemoveClient(Client* client);

    sf::TcpListener _listener;
    sf::SocketSelector _selector;
    std::vector<std::unique_ptr<Client>> _clients;
};
