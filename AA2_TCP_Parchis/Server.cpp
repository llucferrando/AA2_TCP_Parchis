#include "Server.h"


Server::Server(unsigned short port)
{
	if (_listener.listen(port) != sf::Socket::Status::Done) {
		std::cerr << "Error while trying to listen the port " << port << std::endl;
	}
	_selector.add(_listener);
	std::cout << "Server listening to the port " << port << std::endl;
}

void Server::Run()
{
    while (true)
    {
        if (_selector.wait(sf::seconds(1.f)))
        {
            if (_selector.isReady(_listener))
            {
                AcceptNewConnection();
            }
            else
            {
                for (auto& client : _clients)
                {
                    if (_selector.isReady(*client->GetSocket()))
                    {
                        ReceiveData(client.get());
                    }
                }
            }
        }
    }
}

void Server::AcceptNewConnection()
{
    sf::TcpSocket* socket = new sf::TcpSocket();
 
    if (_listener.accept(*socket) == sf::Socket::Status::Done)
    {
        socket->setBlocking(false);
        _selector.add(*socket);
        _clients.emplace_back(std::make_unique<Client>(socket));

        std::cout << "New connection accepted " << socket->getRemoteAddress().value() << std::endl;
    }
    else
    {
        delete socket;
    }
}

void Server::ReceiveData(Client* client)
{
    sf::Packet packet;
    if (client->GetSocket()->receive(packet) == sf::Socket::Status::Done)
    {
        std::string msg;
        packet >> msg;

        std::cout << "[RECEIVED] " << msg << " (from " << client->GetGUID() << ")" << std::endl;

        // Aquí irá la lógica futura: interpretar comandos LOGIN / REGISTER / etc.
    }
    else
    {
        std::cout << "[INFO] Cliente desconectado: " << client->GetGUID() << std::endl;
        RemoveClient(client);
    }
}

void Server::RemoveClient(Client* target)
{
    _selector.remove(*target->GetSocket());

    _clients.erase(
        std::remove_if(_clients.begin(), _clients.end(),
            [target](const std::unique_ptr<Client>& c) {
                return c.get() == target;
            }),
        _clients.end());
}
