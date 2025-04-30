#include "Client.h"
#include <iostream>

Client::Client() {}

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

bool Client::ConnectToBootstrapServer(const std::string& ip, unsigned short port)
{
    auto resolved = sf::IpAddress::resolve(ip);

    if (_bootstrapSocket.connect(*resolved, port) != sf::Socket::Status::Done)
    {
        std::cout << "[Client] Failed to connect to bootstrap server." << std::endl;
        return false;
    }

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

    sf::Packet packet;
    packet << "CREATE_ROOM" << roomID;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool Client::JoinRoom(std::string roomId)
{
    std::cout << "[Client] Join Room request with id " << roomId << std::endl;

    sf::Packet packet;
    packet << "JOIN_ROOM" << roomId;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool Client::ReceivePacket(sf::Packet& packet)
{
    return _bootstrapSocket.receive(packet) == sf::Socket::Status::Done;
}

void Client::StartP2PListening(unsigned short port)
{
    if (_p2pListener.listen(port) != sf::Socket::Status::Done)
    {
        std::cout << "[Client] Failed to start P2P listening on port " << port << std::endl;
    }
}

void Client::ConnectToPeer(const sf::IpAddress& ip, unsigned short port)
{
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
    for (auto& peer : _peers)
    {
        if (peer.socket)
            peer.socket->send(packet);
    }
}