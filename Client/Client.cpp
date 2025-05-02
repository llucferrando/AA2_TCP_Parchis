#include "Client.h"
#include <iostream>

Client::Client() { std::srand(std::time(nullptr)); }

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

void Client::StartP2PListening(unsigned short port)
{
    if (_p2pListener.listen(port) != sf::Socket::Status::Done)
    {
        std::cout << "[Client] Failed to start P2P listening on port " << port << std::endl;
    }

    std::cout << "[Client] Listening on port: " << port << std::endl;
}

void Client::StopP2PListening()
{
    _p2pListener.close();
    std::cout << "[Client] Listener cerrado correctamente." << std::endl;
}

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

bool Client::CreateRoom(std::string roomID)
{
    std::cout << "[Client] Create Room request with id " << roomID << std::endl;
    unsigned short p2pPort = 60000 + rand() % 1000;
    _lastP2PPort = p2pPort;
    StartP2PListening(p2pPort);

    sf::Packet packet;
    packet << "CREATE_ROOM" << roomID << p2pPort;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}
bool Client::ReceivePacketFromPeers(sf::Packet& packet)
{
    for (auto& [socket, ip, port] : _peers)
    {
        if (socket->receive(packet) == sf::Socket::Status::Done)
        {
            std::cout << "[Client] Packet received from peer " << ip << ":" << port << std::endl;
            return true;
        }
    }
    return false;
}
bool Client::JoinRoom(std::string roomId)
{
    std::cout << "[Client] Join Room request with id " << roomId << std::endl;

    unsigned short p2pPort = 60000 + rand() % 1000;
    _lastP2PPort = p2pPort;
    StartP2PListening(p2pPort);

    sf::Packet packet;
    packet << "JOIN_ROOM" << roomId << p2pPort;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool Client::ReceivePacket(sf::Packet& packet)
{
    return _bootstrapSocket.receive(packet) == sf::Socket::Status::Done;
}

void Client::ConnectToPeer(const sf::IpAddress& ip, unsigned short port)
{
    std::cout << "[Client] Trying to connect to peer: " << ip << ":" << port << std::endl;

    sf::TcpSocket* peerSocket = new sf::TcpSocket();

    if (peerSocket->connect(ip, port) == sf::Socket::Status::Done)
    {
        _peers.emplace_back(peerSocket, ip, port);
    }
    else
    {
        std::cout << "[Client] Failed to connect to peer: " << ip << ":" << port << std::endl;
        delete peerSocket;
    }
}

void Client::BroadcastToPeers(sf::Packet& packet)
{
    std::cout << "[Client] Trying to broadcast " << std::endl;
    for (auto [socket, ip, port] : _peers)
    {
        std::cout << ip << " : " << port << std::endl;
    }
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

std::optional<sf::Packet> Client::CheckServerMessage()
{
    if (_selector.wait(sf::milliseconds(0)))
    {
        if (_selector.isReady(_bootstrapSocket))
        {
            sf::Packet packet;
            if (_bootstrapSocket.receive(packet) == sf::Socket::Status::Done)
            {
                return packet;
            }
        }
    }
    return std::nullopt;
}