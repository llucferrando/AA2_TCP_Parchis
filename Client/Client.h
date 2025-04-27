#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <string>

class Client
{
public:
    Client();
    ~Client();

    bool ConnectToBootstrapServer(const std::string& ip, unsigned short port);
    void DisconnectFromBootstrapServer();

    bool SendLogin(const std::string& username, const std::string& password);
    bool SendRegister(const std::string& username, const std::string& password);

    bool CreateRoom();
    bool JoinRoom(int roomId);

    bool ReceivePacket(sf::Packet& packet);
    void StartP2PListening(unsigned short port); // Empezar a aceptar conexiones P2P
    void ConnectToPeer(const sf::IpAddress& ip, unsigned short port); // Conectar a otro jugador
    void BroadcastToPeers(sf::Packet& packet); // Enviar a todos los peers

private:
    sf::TcpSocket _bootstrapSocket;
    sf::TcpListener _p2pListener;
    std::vector<sf::TcpSocket*> _peers;
};

