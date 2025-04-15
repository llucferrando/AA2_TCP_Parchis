#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <sstream>

class Client
{
public:
    Client(sf::TcpSocket* socket);

    sf::TcpSocket* GetSocket();
    std::string GetGUID() const;
    std::string GetNickname() const;

    void SetNickname(const std::string& nickname);

private:
    sf::TcpSocket* _socket;
    std::string _guid;
    std::string _nickname;
};
