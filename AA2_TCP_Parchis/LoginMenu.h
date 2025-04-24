#pragma once
#include "MenuInterface.h"
#include "TextField.h"
#include "Button.h"
#include "GameObject.h"
#include "SpriteRenderer.h"

class LoginMenu : public MenuInterface {
public:
    LoginMenu();
    ~LoginMenu();

    void Init() override;
    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow* window) override;

private:
    sf::RenderWindow* _window;

    GameObject* _backgroundGO;
    TextField* _nicknameField;
    TextField* _passwordField;
    Button* _loginButton;
    Button* _registerButton;
};
