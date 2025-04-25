#include <SFML/Network.hpp>
#include <iostream>

int main() {
    sf::TcpSocket socket;

    const std::string ip = "127.0.0.1";
    auto resolved = sf::IpAddress::resolve(ip);
    if (socket.connect(*resolved, 50000) != sf::Socket::Status::Done) {
        std::cout << "Error connecting to server\n";
        return -1;
    }

    // Enviamos un comando LOGIN
    sf::Packet packet;
    packet << "LOGIN";
    socket.send(packet);

    // Enviamos nickname y password
    sf::Packet credentials;
    credentials << "testuser" << "password";
    socket.send(credentials);

    // Recibimos la respuesta
    sf::Packet response;
    if (socket.receive(response) == sf::Socket::Status::Done) {
        std::string reply;
        response >> reply;
        std::cout << "Respuesta del server: " << reply << std::endl;
    }

    return 0;
}