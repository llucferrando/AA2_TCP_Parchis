#include "BootstrapServer.h"
#include <iostream>

// -- Sets up the listening port and connects to DDBB

BootstrapServer::BootstrapServer(unsigned short port) 
{
    std::srand(std::time(nullptr));

    // Opens port to listen to connection

    if (_listener.listen(port) != sf::Socket::Status::Done) 
    {
        std::cerr << "[Server] Failed to bind port" << std::endl;
    }

    _selector.add(_listener);

    _db.ConnectDatabase();
}

// -- Handles Connections, new clients and room managment

void BootstrapServer::Run() 
{
    while (true) 
    {
        if (_selector.wait(sf::seconds(1.f))) 
        {
            if (_selector.isReady(_listener))
            {
                AcceptNewConnection();
            }
            else 
            {
                for (auto& client : _clients) 
                {
                    if (_selector.isReady(*client->GetSocket())) 
                    {
                        ReceiveData(client.get());
                    }
                }
            }
        }
        for (auto& [roomID, room] : _rooms)
        {
            if (room->waitingToStart && room->GetPlayers().size() >= 2)
            {
                if (room->startTimer.getElapsedTime().asSeconds() >= 10.f)
                {
                    std::cout << "[Server] Timer expired, starting match for room: " << roomID << std::endl;
                    StartMatch(room.get());
                    break;
                }
            }
        }
    }
}

// -- Accepts new clients and adds to the selector

void BootstrapServer::AcceptNewConnection() {
    sf::TcpSocket* socket = new sf::TcpSocket();

    if (_listener.accept(*socket) == sf::Socket::Status::Done) 
    {
        socket->setBlocking(false);
        _selector.add(*socket);
        _clients.emplace_back(std::make_unique<Client>(socket));
        std::cout << "[Server] New client connected." << std::endl;
    }
    else 
    {
        delete socket;
    }
}

// -- Receives data from a client and processes the command

void BootstrapServer::ReceiveData(Client* client) 
{
    sf::Packet packet;

    if (client->GetSocket()->receive(packet) == sf::Socket::Status::Done)
    {
        std::cout << "[Server] Packet received" << std::endl;

        std::string command;
        if (!(packet >> command))
        {
            std::cerr << "[Server] Failed to extract command" << std::endl;
            return;
        }

        std::cout << "[Server] Command: " << command << std::endl;

        if (command == "LOGIN" || command == "REGISTER")
        {
            std::string nick, pass;
            if (!(packet >> nick >> pass)) {
                std::cerr << "[Server] Failed to extract user & password" << std::endl;
                return;
            }

            HandleCommand(client, command, nick, pass);
        }
        else
        {
            HandleCommand(client, command, packet);
        }
    }
}

// -- Handles Login and Regfitrr checking DDBBManager

void BootstrapServer::HandleCommand(Client* client, const std::string& command, const std::string& nick, const std::string& pass)
{
    sf::Packet response;

    if (command == "REGISTER")
    {
        bool success = _db.RegisterUser(nick, pass);
        response << (success ? "REGISTER_OK" : "REGISTER_FAIL");

        client->GetSocket()->send(response);
    }
    else if (command == "LOGIN")
    {
        bool success = _db.LoginUser(nick, pass);
        response << (success ? "LOGIN_OK" : "LOGIN_FAIL");

        client->GetSocket()->send(response);
    }
}

// -- Handles Join romm and Create Room

void BootstrapServer::HandleCommand(Client* client, const std::string& command, sf::Packet& packet)
{
    if (command.empty()) return;

    std::string roomID;
    unsigned short p2pPort;
    packet >> roomID >> p2pPort;

    client->SetP2PPort(p2pPort);

    if (command == "JOIN_ROOM")
    {
        std::cout << "[Server] Join room requested with ID: " << roomID << std::endl;

        JoinRoom(client, roomID);
    }
    else if(command == "CREATE_ROOM")
    {
        CreateRoom(client, roomID);
    }
    else
    {
        std::cout << "[Server] Unknown command: " << command << std::endl;
    }
}

// -- Removes Client from the selector

void BootstrapServer::RemoveClient(Client* client) 
{
    _selector.remove(*client->GetSocket());

    _clients.erase(std::remove_if(_clients.begin(), _clients.end(), [client](const std::unique_ptr<Client>& c) { return c.get() == client; }), _clients.end());
}


#pragma region Join & Create Room

// -- Creates a new room, assigns a unique ID if needed and adds the client

void BootstrapServer::CreateRoom(Client* client, const std::string& roomID) 
{
    std::cout << "[Server] Creating a room..." << std::endl;

    std::string finalRoomID = roomID.empty() ? GenerateRandomRoomID() : roomID;

    if (_rooms.find(finalRoomID) != _rooms.end()) 
    {
        std::cout << "[Server] Room ID already exists: " << finalRoomID << ". Generating new ID..." << std::endl;
        finalRoomID = GenerateRandomRoomID();
    }

    _rooms[finalRoomID] = std::make_unique<Room>(finalRoomID, 4);

    _rooms[finalRoomID]->AddPlayer(client);

    client->SetRoomID(finalRoomID);

    std::cout << "[Server] Room created with ID: " << finalRoomID << std::endl;
}

// -- Adds a client to an existing room and notifies all peers

void BootstrapServer::JoinRoom(Client* client, const std::string& roomID)
{
    std::cout << "[Server] Join room requested with ID: " << roomID << std::endl;

    if (!client || client->GetRoomID() == roomID)
    {
        std::cout << "[Server] Client already in room or invalid.\n";
        return;
    }

    auto it = _rooms.find(roomID);

    if (it != _rooms.end() && !it->second->IsFull())
    {
        Room* room = it->second.get();
        room->AddPlayer(client);
        client->SetRoomID(roomID);

        const auto& players = room->GetPlayers();
        int otherPlayers = players.size() - 1;

        // Enviar al nuevo cliente los peers ya conectados
        sf::Packet response;
        response << "JOIN_OK" << otherPlayers;

        for (auto* player : players)
        {
            if (player != client)
            {
                auto optionalIp = player->GetSocket()->getRemoteAddress();
                if (optionalIp.has_value())
                {
                    sf::IpAddress ip = optionalIp.value();
                    unsigned short port = player->GetP2PPort();
                    response << ip.toString() << port;
                }
            }
        }

        client->GetSocket()->send(response);

        // Notificar a los otros jugadores del nuevo peer
        sf::Packet notify;
        auto optionalIp = client->GetSocket()->getRemoteAddress();
        if (optionalIp.has_value())
        {
            notify << "NEW_PEER" << optionalIp.value().toString() << client->GetP2PPort();

            for (auto* player : players)
            {
                if (player != client)
                {
                    player->GetSocket()->send(notify);
                }
            }
        }

        int numPlayers = players.size();

        if (numPlayers == 2 && !room->waitingToStart)
        {
            room->waitingToStart = true;
            room->startTimer.restart();
            std::cout << "[Server] Room has 2 players, starting 10s countdown..." << std::endl;
        }
        else if (numPlayers == 4)
        {
            std::cout << "[Server] Room full, starting immediately." << std::endl;
            StartMatch(room);
        }
    }
    else
    {
        std::cout << "[Server] This room does not exist or is full: " << roomID << std::endl;
        sf::Packet errorPacket;
        errorPacket << "JOIN_FAIL";
        client->GetSocket()->send(errorPacket);
    }
}

// -- If player sends a unvalid room ID generates one randomly

std::string BootstrapServer::GenerateRandomRoomID()
{
    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string roomId;
    for (int i = 0; i < 6; ++i)
    {
        roomId += charset[rand() % charset.size()];
    }
    return roomId;
}

#pragma endregion

// -- Starts a match for a room: notifies players and closes connections

void BootstrapServer::StartMatch(Room* room)
{
    std::cout << "[Server] Match started for room: " << room->GetID() << std::endl;

    auto playersCopy = room->GetPlayers();

    for (int i = 0; i < playersCopy.size(); ++i)
    {
        sf::Packet packet;
        packet << "START_P2P";

        int playerIndex = i;
        int numPlayers = playersCopy.size();

        packet << playerIndex << numPlayers;

        playersCopy[i]->GetSocket()->send(packet);
    }

    for (auto* player : playersCopy)
    {
        _selector.remove(*player->GetSocket());
        player->GetSocket()->disconnect();
        RemoveClient(player);
    }

    _rooms.erase(room->GetID());
}

