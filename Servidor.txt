#include <SFML/Network.hpp>
#include <iostream>

#define SERVER_PORT 55000
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);


void main() {
	sf::TcpSocket socketServer;
	bool gameLoop = true;

	if (socketServer.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done) {
		std::cerr << "Error al conectar con el servidor" << std::endl;
	}
	else {
		std::cout << "Conectado con el servidor" << std::endl;
		while (gameLoop) {
			sf::sleep(sf::seconds(1));
		}

		socketServer.disconnect();
		std::cout << "Desconectado del servidor" << std::endl;
	}
}