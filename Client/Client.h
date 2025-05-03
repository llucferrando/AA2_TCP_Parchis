#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include "Event.h"

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

    // -- BootstrapServer
    bool ConnectToBootstrapServer(const std::string& ip, unsigned short port);
    void DisconnectFromBootstrapServer();
    bool ReceivePacketFromServer(sf::Packet& packet);
    std::optional<sf::Packet> CheckServerMessage();
    void HandleServerMessages(Event<> OnStartMatch);

    // -- Matchmaking & Login
    bool SendLogin(const std::string& username, const std::string& password);
    bool SendRegister(const std::string& username, const std::string& password);
    bool CreateRoom(std::string roomID);
    bool JoinRoom(std::string roomId);
    std::optional<sf::Packet> WaitForServerMessage(float timeoutSeconds);

    // -- P2P
    std::optional<sf::Packet> WaitForPeerMessage(float timeoutSeconds);
    void StartListeningForPeers();
    bool ReceivePacketFromPeers(sf::Packet& packet);
    void ConnectToPeer(const sf::IpAddress& ip, unsigned short port); 
    void BroadcastToPeers(sf::Packet& packet); 
    void UpdateP2PConnections();

    // -- Getters & Setters
    void SetPlayerIndex(int index);
    void SetNumPlayers(int num);
    int GetPlayerIndex() const;
    int GetNumPlayers() const;
    sf::SocketSelector GetSelector();

    sf::TcpListener _p2pListener;
private:
    sf::TcpSocket _bootstrapSocket;

    int _playerIndex = -1;
    int _numPlayers = 1;
    
    std::vector<PeerInfo> _peers;
    
    unsigned short _p2pPort;

    sf::SocketSelector _selector;
};

