#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(EventHandler* eventHandler, Client* client, std::string spritePath)
{
    AddComponent<SpriteRenderer>(spritePath, sf::Color::White, false);

    _exitRoomMenuButton = AddComponent<ButtonComponent>(sf::Vector2f(120, 450), sf::Vector2f(250, 50), "Exit To Menu", eventHandler);

    _exitGameButton = AddComponent<ButtonComponent>(sf::Vector2f(420, 450), sf::Vector2f(250, 50), "Exit Game", eventHandler);

    _client = client;

    _exitRoomMenuButton->onClick.Subscribe([this]() {  onReturnMenu.Invoke();  });

    _exitGameButton->onClick.Subscribe([this]() { onExitGame.Invoke(); });
}

void GameOverMenu::Update(float deltaTime)
{
}

void GameOverMenu::Render(sf::RenderWindow* window)
{
    GetComponent<SpriteRenderer>()->Draw(window, GetComponent<Transform>());
    _exitRoomMenuButton->Render(window);
    _exitGameButton->Render(window);
}

ButtonComponent* GameOverMenu::GetExitToRoomButton()
{
    return _exitRoomMenuButton;
}

ButtonComponent* GameOverMenu::GetExitGameButton()
{
    return _exitGameButton;
}


void GameOverMenu::SubcribeToButtons()
{


}