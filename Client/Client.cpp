#include "Client.h"
#include <iostream>

Client::Client() {}

Client::~Client()
{
    DisconnectFromBootstrapServer();

    for (auto& peer : _peers)
    {
        peer->disconnect();
        delete peer;
    }
    _peers.clear();
}

bool Client::ConnectToBootstrapServer(const std::string& ip, unsigned short port)
{
    auto resolved = sf::IpAddress::resolve(ip);

    if (_bootstrapSocket.connect(*resolved, port) != sf::Socket::Status::Done)
    {
        std::cout << "Failed to connect to bootstrap server." << std::endl;
        return false;
    }
    return true;
}

void Client::DisconnectFromBootstrapServer()
{
    _bootstrapSocket.disconnect();
}

bool Client::SendLogin(const std::string& username, const std::string& password)
{
    std::cout << "Hola mi user es:" << username << " " << password << std::endl;
    sf::Packet packet;
    packet << "LOGIN" << username << password;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool Client::SendRegister(const std::string& username, const std::string& password)
{
    sf::Packet packet;
    packet << "REGISTER" << username << password;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool Client::CreateRoom()
{
    sf::Packet packet;
    packet << "CREATE_ROOM";
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool Client::JoinRoom(int roomId)
{
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
        std::cout << "Failed to start P2P listening on port " << port << std::endl;
    }
}

void Client::ConnectToPeer(const sf::IpAddress& ip, unsigned short port)
{
    sf::TcpSocket* peer = new sf::TcpSocket();
    if (peer->connect(ip, port) == sf::Socket::Status::Done)
    {
        _peers.push_back(peer);
    }
    else
    {
        std::cout << "Failed to connect to peer: " << ip << ":" << port << std::endl;
        delete peer;
    }
}

void Client::BroadcastToPeers(sf::Packet& packet)
{
    for (auto& peer : _peers)
    {
        peer->send(packet);
    }
}