#include "MatchmakingMenu.h"
#include <iostream>

MatchmakingMenu::MatchmakingMenu(EventHandler* eventHandler, Client* client)
{
	AddComponent<SpriteRenderer>("Assets/Splashscreen/matchmaking.png", sf::Color::White);

	_idCreateRoomField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 300), sf::Vector2f(400, 50), "ID create room", eventHandler);

	_idJoinRoomField = AddComponent<TextFieldComponent>(sf::Vector2f(160, 450), sf::Vector2f(400, 50), "ID join room", eventHandler);

	_createRoomButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 370), sf::Vector2f(180, 50), "Create", eventHandler);

	_joinRoomButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 520), sf::Vector2f(180, 50), "Join", eventHandler);

	_client = client;

	_createRoomButton->onClick.Subscribe([this]() {
		std::cout << "[Client] Creating a room..." << std::endl;

		if (_client->CreateRoom(_idCreateRoomField->GetText()))
		{
			std::cout << "[Client] Room created successfully, now listening for players..." << std::endl;
			_waitingForStartP2P = true;
		}
		else
		{
			std::cout << "[Client] Failed to create room." << std::endl;
		}
		});

	_joinRoomButton->onClick.Subscribe([this]() {
		std::cout << "[Client] Join room requested..." << std::endl;

		if (_client->JoinRoom(GetJoinIDText())) {
			std::cout << "[Client] Join packet sent. Waiting for response..." << std::endl;
			_waitingForStartP2P = true;
		}
		else {
			std::cout << "[Client] Failed to send join request." << std::endl;
		}
		});
}

void MatchmakingMenu::Update(float deltaTime)
{
	_idCreateRoomField->Update(deltaTime);
	_idJoinRoomField->Update(deltaTime);

	if (_waitingForStartP2P)
	{
		auto optPacket = _client->CheckServerMessage();
		if (optPacket.has_value())
		{
			sf::Packet& packet = optPacket.value();

			std::string header;
			packet >> header;

			if (header == "JOIN_FAIL") {
				std::cout << "[Client] This room does not exist." << std::endl;
				_waitingForStartP2P = false;
				return;
			}

			if (header == "JOIN_OK") {
				int numPeers;
				packet >> numPeers;

				for (int i = 0; i < numPeers; ++i)
				{
					std::string ipString;
					unsigned short port;
					packet >> ipString >> port;

					auto resolved = sf::IpAddress::resolve(ipString);
					sf::IpAddress ip(*resolved);
					_client->ConnectToPeer(ip, port);
				}
				std::cout << "[Client] Connected to all peers!" << std::endl;
			}

			if (header == "NEW_PEER") {
				std::string ipString;
				unsigned short port;
				packet >> ipString >> port;

				auto resolved = sf::IpAddress::resolve(ipString);
				sf::IpAddress ip(*resolved);
				_client->ConnectToPeer(ip, port);

				std::cout << "[Client] Received new peer: " << ip << ":" << port << std::endl;
			}

			if (header == "START_P2P") {
				int index, total;
				packet >> index >> total;

				_client->SetPlayerIndex(index);
				_client->SetNumPlayers(total);
				sf::sleep(sf::milliseconds(300)); // por seguridad antes de cerrar el listener
				_client->StopP2PListening();

				std::cout << "[Client] Soy el jugador " << index << " de " << total << std::endl;

				_waitingForStartP2P = false;
				onStartMatch.Invoke();
			}
		}
	}
}

void MatchmakingMenu::Render(sf::RenderWindow* window)
{
	GetComponent<SpriteRenderer>()->Draw(window, GetComponent<Transform>());
	_idCreateRoomField->Render(window);
	_idJoinRoomField->Render(window); 
	_createRoomButton->Render(window);
	_joinRoomButton->Render(window);
}

ButtonComponent* MatchmakingMenu::GetCreateRoomButton() { return _createRoomButton; }

ButtonComponent* MatchmakingMenu::GetJoinRoomButton() { return _joinRoomButton; }

std::string MatchmakingMenu::GetJoinIDText() { return _idJoinRoomField->GetText(); }

std::string MatchmakingMenu::GetCreateIDText() { return _idCreateRoomField->GetText(); }
