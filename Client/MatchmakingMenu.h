#pragma once
#include "Menu.h"
#include "ButtonComponent.h"
#include "TextFieldComponent.h"
#include "SpriteRenderer.h"
#include "EventHandler.h"
#include "Client.h"

class MatchmakingMenu : public Menu
{
public:
    MatchmakingMenu(EventHandler* eventHandler, Client* client);

    void Update(float deltaTime);
    void Render(sf::RenderWindow* window);
    ButtonComponent* GetCreateRoomButton();
    ButtonComponent* GetJoinRoomButton();
    std::string GetCreateIDText();
    std::string GetJoinIDText();
    Event<> onStartMatch;

private:
    TextFieldComponent* _idCreateRoomField;
    TextFieldComponent* _idJoinRoomField;
    ButtonComponent* _createRoomButton;
    ButtonComponent* _joinRoomButton;
    Client* _client;
};

