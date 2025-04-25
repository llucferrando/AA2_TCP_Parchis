#include "BootstrapServer.h"
#include <iostream>

BootstrapServer::BootstrapServer(unsigned short port) 
{

    if (_listener.listen(port) != sf::Socket::Status::Done) 
    {
        std::cerr << "Failed to bind port." << std::endl;
    }
    _selector.add(_listener);
    _db.ConnectDatabase();
}

void BootstrapServer::Run() {
    while (true) {
        if (_selector.wait(sf::seconds(1.f))) {
            if (_selector.isReady(_listener)) AcceptNewConnection();
            else {
                for (auto& client : _clients) {
                    if (_selector.isReady(*client->GetSocket())) {
                        ReceiveData(client.get());
                    }
                }
            }
        }
    }
}

void BootstrapServer::AcceptNewConnection() {
    sf::TcpSocket* socket = new sf::TcpSocket();
    if (_listener.accept(*socket) == sf::Socket::Status::Done) {
        socket->setBlocking(false);
        _selector.add(*socket);
        _clients.emplace_back(std::make_unique<Client>(socket));
        std::cout << "New client connected." << std::endl;
    }
    else {
        delete socket;
    }
}

void BootstrapServer::ReceiveData(Client* client) {
    sf::Packet packet;
    if (client->GetSocket()->receive(packet) == sf::Socket::Status::Done) {
        std::string command;
        packet >> command;
        HandleCommand(client, command);
    }
    else {
        RemoveClient(client);
    }
}

void BootstrapServer::HandleCommand(Client* client, const std::string& command) {
    sf::Packet response;
    if (command == "REGISTER") {
        std::string nick, pass;
        client->GetSocket()->receive(response);
        response >> nick >> pass;
        bool success = _db.RegisterUser(nick, pass);
        response.clear();
        response << (success ? "REGISTER_OK" : "REGISTER_FAIL");
        client->GetSocket()->send(response);
    }
    else if (command == "LOGIN") {
        std::string nick, pass;
        client->GetSocket()->receive(response);
        response >> nick >> pass;
        bool success = _db.LoginUser(nick, pass);
        response.clear();
        response << (success ? "LOGIN_OK" : "LOGIN_FAIL");
        client->GetSocket()->send(response);
    }
    else if (command.rfind("CREATE_ROOM:", 0) == 0) {
        std::string roomID = command.substr(12);
        CreateRoom(client, roomID);
    }
    else if (command.rfind("JOIN_ROOM:", 0) == 0) {
        std::string roomID = command.substr(10);
        JoinRoom(client, roomID);
    }
}

void BootstrapServer::CreateRoom(Client* client, const std::string& roomID) {
    if (_rooms.find(roomID) != _rooms.end()) return;
    _rooms[roomID] = std::make_unique<Room>(roomID);
    _rooms[roomID]->AddPlayer(client);
    client->SetRoomID(roomID);
}

void BootstrapServer::JoinRoom(Client* client, const std::string& roomID) {
    auto it = _rooms.find(roomID);
    if (it != _rooms.end() && !it->second->IsFull()) {
        it->second->AddPlayer(client);
        client->SetRoomID(roomID);
        if (it->second->IsFull()) StartMatch(it->second.get());
    }
}

void BootstrapServer::StartMatch(Room* room) {
    const auto& players = room->GetPlayers();
    for (auto* player : players) {
        sf::Packet packet;
        packet << "START_P2P";
        player->GetSocket()->send(packet);
        _selector.remove(*player->GetSocket());
        player->GetSocket()->disconnect();
    }
    _rooms.erase(room->GetID());
}

void BootstrapServer::RemoveClient(Client* client) {
    _selector.remove(*client->GetSocket());
    _clients.erase(std::remove_if(_clients.begin(), _clients.end(), [client](const std::unique_ptr<Client>& c) {
        return c.get() == client;
        }), _clients.end());
}
