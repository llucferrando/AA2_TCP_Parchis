#include "Client.h"
#include <iostream>

Client::Client() 
{ 
    std::srand(std::time(nullptr)); 
    _p2pPort = 60000 + rand() % 1000; // -- Random port for single player same PC
}

// -- Disconnects from Server and all peers and frees memory

Client::~Client()
{
    DisconnectFromBootstrapServer();

    for (auto& peer : _peers)
    {
        if (peer.socket)
        {
            peer.socket->disconnect();
            delete peer.socket;
        }
    }
    _peers.clear();
}



#pragma region BootstrapServer

// -- Connects to server and adds the socket to the selector

bool Client::ConnectToBootstrapServer(const std::string& ip, unsigned short port)
{
    auto resolved = sf::IpAddress::resolve(ip);

    if (_bootstrapSocket.connect(*resolved, port) != sf::Socket::Status::Done)
    {
        std::cout << "[Client] Failed to connect to bootstrap server." << std::endl;
        return false;
    }

    _bootstrapSocket.setBlocking(false);
    _selector.add(_bootstrapSocket);

    std::cout << "[Client] Connected to bootstrap server ip " + ip << std::endl;
    return true;
}

void Client::DisconnectFromBootstrapServer()
{
    std::cout << "[Client] Disconnected from bootstrap server." << std::endl;
    _bootstrapSocket.disconnect();
}

bool Client::SendLogin(const std::string& username, const std::string& password)
{
    std::cout << "[Client] Login sended with user " << username << " and password " << password << std::endl;
    sf::Packet packet;
    packet << "LOGIN" << username << password;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool Client::SendRegister(const std::string& username, const std::string& password)
{
    std::cout << "[Client] Register sended with user " << username << " and password " << password << std::endl;

    sf::Packet packet;
    packet << "REGISTER" << username << password;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

// -- Sends request to the server to create room with specific ID and port

bool Client::CreateRoom(std::string roomID)
{
    std::cout << "[Client] Create Room request with id " << roomID << std::endl;

    sf::Packet packet;
    packet << "CREATE_ROOM" << roomID << _p2pPort;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

// -- Sens request to the server to join a room with specific ID and port

bool Client::JoinRoom(std::string roomId)
{
    std::cout << "[Client] Join Room request with id " << roomId << std::endl;

    sf::Packet packet;
    packet << "JOIN_ROOM" << roomId << _p2pPort;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

// -- Given a timeout (blocking) wait for a package sended from the server

std::optional<sf::Packet> Client::WaitForServerMessage(float timeoutSeconds)
{
    if (_selector.wait(sf::seconds(timeoutSeconds)) && _selector.isReady(_bootstrapSocket))
    {
        sf::Packet packet;
        if (_bootstrapSocket.receive(packet) == sf::Socket::Status::Done)
        {
            return packet;
        }
    }
    return std::nullopt;
}

// -- Recieves a package form server without bkloccking

bool Client::ReceivePacketFromServer(sf::Packet& packet)
{
    return _bootstrapSocket.receive(packet) == sf::Socket::Status::Done;
}

//-- Checks if there is any message from server to be read

std::optional<sf::Packet> Client::CheckServerMessage()
{
    if (_selector.isReady(_bootstrapSocket))
    {
        sf::Packet packet;
        if (_bootstrapSocket.receive(packet) == sf::Socket::Status::Done)
        {
            return packet;
        }
    }
    return std::nullopt;
}

// -- Handles all the server messages

void Client::HandleServerMessages(Event<> OnStartMatch)
{
    auto packetOpt = CheckServerMessage();
    if (!packetOpt.has_value()) return;

    sf::Packet packet = packetOpt.value();
    std::string cmd;
    packet >> cmd;

    if (cmd == "JOIN_OK")
    {
        int numPeers;
        packet >> numPeers;
        for (int i = 0; i < numPeers; ++i)
        {
            std::string ipStr;
            unsigned short port;
            packet >> ipStr >> port;

            auto resolved = sf::IpAddress::resolve(ipStr);
            if (resolved.has_value())
            {
                ConnectToPeer(resolved.value(), port);
            }
            else
            {
                std::cout << "[Client] Failed to resolve peer IP: " << ipStr << std::endl;
            }
        }
    }
    else if (cmd == "NEW_PEER")
    {
        std::string ipStr;
        unsigned short port;
        packet >> ipStr >> port;

        auto resolved = sf::IpAddress::resolve(ipStr);
        if (resolved.has_value())
        {
            ConnectToPeer(resolved.value(), port);
            std::cout << "[Client] Connected to new peer announced by server: " << ipStr << ":" << port << std::endl;

        }
        else
        {
            std::cout << "[Client] Failed to resolve new peer IP: " << ipStr << std::endl;
        }
    }
    else if (cmd == "JOIN_FAIL")
    {
        std::cout << "[Client] Failed to join room (JOIN_FAIL)." << std::endl;
    }

    else if (cmd == "START_P2P")
    {
        int playerIndex;
        int numPlayers;
        packet >> playerIndex >> numPlayers;

        std::cout << "[Client] Match starting! Player index: " << playerIndex << ", total players: " << numPlayers << std::endl;

        _playerIndex = playerIndex;
        _numPlayers = numPlayers;
        OnStartMatch.Invoke();
    }
}

#pragma endregion

#pragma region P2P

void Client::ClearPeers()
{
    for (auto& peer : _peers)
    {
        if (peer.socket)
        {
            peer.socket->disconnect();
            delete peer.socket;
        }
    }
    _peers.clear();
}

// -- Waits for a message from any peer for a limited time

std::optional<sf::Packet> Client::WaitForPeerMessage(float timeoutSeconds)
{
    _selector.wait(sf::seconds(timeoutSeconds));

    for (auto it = _peers.begin(); it != _peers.end(); )
    {
        auto& [socket, ip, port] = *it;

        if (_selector.isReady(*socket))
        {
            sf::Packet packet;
            sf::Socket::Status status = socket->receive(packet);

            if (status == sf::Socket::Status::Done)
            {
                std::cout << "[Client] Packet received from peer " << ip << ":" << port << std::endl;
                return packet;
            }
            else if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error)
            {
                std::cout << "[Client] Peer disconnected: " << ip << ":" << port << std::endl;
                _selector.remove(*socket);
                delete socket;
                it = _peers.erase(it);
                continue;
            }
        }

        ++it;
    }

    return std::nullopt;
}

void Client::StartListeningForPeers()
{
    if (_p2pListener.listen(_p2pPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[Client] Failed to listen on P2P port " << _p2pPort << std::endl;
    }
    else
    {
        std::cout << "[Client] Listening for P2P on port " << _p2pPort << std::endl;
        _selector.add(_p2pListener);
    }
}

// -- Connects to a new peer if not already connected

void Client::ConnectToPeer(const sf::IpAddress& ip, unsigned short port)
{
    if (ip == sf::IpAddress::getLocalAddress() && port == _p2pPort)
    {
        std::cout << "[Client] Skipping self-connection to " << ip << ":" << port << std::endl;
        return;
    }

    // Verifica que no est�s ya conectado a este peer
    for (const auto& peer : _peers)
    {
        if (peer.ip == ip && peer.port == port)
        {
            std::cout << "[Client] Already connected to peer: " << ip << ":" << port << std::endl;
            return;
        }
    }

    std::cout << "[Client] Trying to connect to peer: " << ip << ":" << port << std::endl;

    sf::TcpSocket* peerSocket = new sf::TcpSocket();

    if (peerSocket->connect(ip, port) == sf::Socket::Status::Done)
    {
        peerSocket->setBlocking(false);
        _peers.emplace_back(peerSocket, ip, port);
        _selector.add(*peerSocket);
        std::cout << "[Client] Connected to peer: " << ip << ":" << port << std::endl;
    }
    else
    {
        std::cout << "[Client] Failed to connect to peer: " << ip << ":" << port << std::endl;
        delete peerSocket;
    }
}

void Client::BroadcastToPeers(sf::Packet& packet)
{
    std::cout << "[Client] Trying to broadcast to " << _peers.size() << " peer(s)." << std::endl;

    for (auto& [socket, ip, port] : _peers)
    {
        if (socket->send(packet) == sf::Socket::Status::Done)
        {
            std::cout << "[Client] Packet sent to peer " << ip << ":" << port << std::endl;
        }
        else
        {
            std::cout << "[Client] Failed to send packet to " << ip << ":" << port << std::endl;
        }
    }
}

// -- Accepts new incoming P2P connections and adds them to the peer list

void Client::UpdateP2PConnections()
{
    if (_selector.isReady(_p2pListener))
    {
        sf::TcpSocket* newPeer = new sf::TcpSocket();
        if (_p2pListener.accept(*newPeer) == sf::Socket::Status::Done)
        {
            newPeer->setBlocking(false);
            auto optionalIp = newPeer->getRemoteAddress();
            unsigned short port = newPeer->getRemotePort();
            if (optionalIp.has_value())
            {
                sf::IpAddress ip = optionalIp.value();

                // Verificar si ya est�s conectado
                for (const auto& peer : _peers)
                {
                    if (peer.ip == ip && peer.port == port)
                    {
                        std::cout << "[Client] Peer already connected: " << ip << ":" << port << std::endl;
                        delete newPeer;
                        return;
                    }
                }

                _peers.emplace_back(newPeer, ip, port);
                _selector.add(*newPeer);
                std::cout << "[Client] New peer connected: " << ip << ":" << port << std::endl;
            }
            else
            {
                std::cout << "[Client] New peer connected but no valid IP." << std::endl;
                delete newPeer;
            }
        }
        else
        {
            delete newPeer;
        }
    }
}

// -- Sends the client's username and index to all peers for showing in game

void Client::SendUsername()
{
    sf::Packet packet;
    packet << static_cast<int>(MessageType::PLAYER_PROFILE)
        << _playerIndex
        << _playerUsername;

    BroadcastToPeers(packet);

    std::cout << "[Client] Sent username '" << _playerUsername << "' as playerIndex " << _playerIndex << " to all peers";

}

bool Client::ReceivePacketFromPeers(sf::Packet& packet)
{
    _selector.wait(sf::Time::Zero);

    for (auto it = _peers.begin(); it != _peers.end(); )
    {
        auto& [socket, ip, port] = *it;

        if (_selector.isReady(*socket))
        {
            sf::Packet temp;
            sf::Socket::Status status = socket->receive(temp);

            if (status == sf::Socket::Status::Done)
            {
                packet = temp;
                std::cout << "[Client] Packet received from peer " << ip << ":" << port << std::endl;
                return true;
            }
            else if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error)
            {
                std::cout << "[Client] Peer disconnected: " << ip << ":" << port << std::endl;
                _selector.remove(*socket);
                delete socket;
                it = _peers.erase(it);
                continue;
            }
        }

        ++it;
    }

    return false;
}

#pragma endregion

#pragma region Getters & Setters

    void Client::SetPlayerIndex(int index) { _playerIndex = index; }

    void Client::SetNumPlayers(int num) { _numPlayers = num; }

    void Client::SetMyUsername(const std::string& name) { _playerUsername = name; }  

    int Client::GetPlayerIndex() const { return _playerIndex; }

    int Client::GetNumPlayers() const { return _numPlayers; }
     
    sf::SocketSelector Client::GetSelector(){ return _selector; }

#pragma endregion