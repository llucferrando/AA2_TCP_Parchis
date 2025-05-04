#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Client.h"
#include "Room.h"
#include "DataBaseManager.h"

class BootstrapServer {
public:
    BootstrapServer(unsigned short port = 50000);
    void Run();

private:

    void AcceptNewConnection();
    void ReceiveData(Client* client);
    
    void HandleCommand(Client* client, const std::string& command, const std::string& nick, const std::string& pass);
    void HandleCommand(Client* client, const std::string& command, sf::Packet& packet);
    
    void CreateRoom(Client* client, const std::string& roomID);
    void JoinRoom(Client* client, const std::string& roomID);
    
    void StartMatch(Room* room);
    void RemoveClient(Client* client);
    
    std::string GenerateRandomRoomID();

    sf::TcpListener _listener;
    sf::SocketSelector _selector;
    std::vector<std::unique_ptr<Client>> _clients;
    std::unordered_map<std::string, std::unique_ptr<Room>> _rooms;
    DataBaseManager _db;
};