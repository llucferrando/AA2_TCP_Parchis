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

		std::cout << "Creating a room..." << std::endl;

		if (_client->CreateRoom(_idCreateRoomField->GetText())) {
			std::cout << "Room created successfully, now listening for players..." << std::endl;
			_client->StartP2PListening(60000); // ejemplo puerto libre
		}
		else {
			std::cout << "Failed to create room." << std::endl;
		}
	});

	_joinRoomButton->onClick.Subscribe([this]() {

		std::cout << "Joining a room requested..." << std::endl;

		if (_client->JoinRoom(GetJoinIDText())) {

			sf::Packet response;

			if (_client->ReceivePacket(response))
			{
				int numPeers;
				response >> numPeers;

				for (int i = 0; i < numPeers; ++i)
				{
					std::string ipString;
					unsigned short port;
					response >> ipString >> port;

					auto resolved = sf::IpAddress::resolve(ipString);

					sf::IpAddress ip(*resolved);
					_client->ConnectToPeer(ip, port);
				}

				std::cout << "Connected to all peers!" << std::endl;
				// Aquí podrías cambiar de estado a Gameplay, etc.
			}
		}
		else {
			std::cout << "Failed to join room." << std::endl;
		}
	});
}

void MatchmakingMenu::Update(float deltaTime)
{
	_idCreateRoomField->Update(deltaTime);
	_idJoinRoomField->Update(deltaTime);
}

void MatchmakingMenu::Render(sf::RenderWindow* window)
{
	GetComponent<SpriteRenderer>()->Draw(window, GetComponent<Transform>());
	_idCreateRoomField->Render(window);
	_idJoinRoomField->Render(window); 
	_createRoomButton->Render(window);
	_joinRoomButton->Render(window);
}

ButtonComponent* MatchmakingMenu::GetCreateRoomButton()
{
	return _createRoomButton;
}

ButtonComponent* MatchmakingMenu::GetJoinRoomButton()
{
	return _joinRoomButton;
}

std::string MatchmakingMenu::GetJoinIDText()
{
	return _idJoinRoomField->GetText();
}

std::string MatchmakingMenu::GetCreateIDText()
{
	return _idCreateRoomField->GetText();
}
