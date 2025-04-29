#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <string>

struct PeerInfo
{
    sf::TcpSocket* socket = nullptr;
    sf::IpAddress ip;
    unsigned short port = 0;

    PeerInfo() = default;
    PeerInfo(sf::TcpSocket* s, sf::IpAddress address, unsigned short p)
        : socket(s), ip(address), port(p) {}
};

class Client
{
public:
    Client();
    ~Client();

    bool ConnectToBootstrapServer(const std::string& ip, unsigned short port);
    void DisconnectFromBootstrapServer();

    bool SendLogin(const std::string& username, const std::string& password);
    bool SendRegister(const std::string& username, const std::string& password);

    bool CreateRoom(std::string roomID);
    bool JoinRoom(std::string roomId);

    bool ReceivePacket(sf::Packet& packet);
    void StartP2PListening(unsigned short port); 
    void ConnectToPeer(const sf::IpAddress& ip, unsigned short port); 
    void BroadcastToPeers(sf::Packet& packet); 

private:
    sf::TcpSocket _bootstrapSocket;
    sf::TcpListener _p2pListener;
    std::vector<PeerInfo> _peers;
};

