#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(EventHandler* eventHandler, Client* client)
{
    AddComponent<SpriteRenderer>("Assets/Splashscreen/gameOverLost.png", sf::Color::White, false);

    _exitRoomMenuButton = AddComponent<ButtonComponent>(sf::Vector2f(170, 450), sf::Vector2f(250, 50), "Exit To Menu", eventHandler);

    _exitGameButton = AddComponent<ButtonComponent>(sf::Vector2f(370, 450), sf::Vector2f(250, 50), "Exit Game", eventHandler);

    _client = client;

    //Subcribe to OnClickEvent for each button in the menu
    SubcribeToButtons();
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

    _exitRoomMenuButton->onClick.Subscribe([this]() {
        std::cout << "[Client] Returning to matchmaking menu..." << std::endl;

        onReturnMenu.Invoke();

        });

    _exitGameButton->onClick.Subscribe([this]() {
        std::cout << "[Client] Quiting the game..." << std::endl;

        onExitGame.Invoke();

        });

}