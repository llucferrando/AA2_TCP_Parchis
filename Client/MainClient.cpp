#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "ButtonComponent.h"
#include "TextFieldComponent.h"
#include "Window.h"
#include <iostream>

int main() {
    Window* _window = new Window();
    _window->GetWindow()->setFramerateLimit(60);

    EventHandler eventHandler;

    // Crear botón
    ButtonComponent button({ 300, 400 }, { 200, 60 }, "Click me", &eventHandler);
    button.onClick.Subscribe([]() {
        std::cout << "Button clicked!" << std::endl;
        });

    ButtonComponent button2({ 100, 200 }, { 100, 20 }, "Click me", &eventHandler);
    button2.onClick.Subscribe([]() {
        std::cout << "Button clicked MARIKON!" << std::endl;
        });

    // Crear campo de texto
    TextFieldComponent textField({ 300, 300 }, { 200, 50 }, "Enter text...", &eventHandler);
    sf::Clock deltaClock;

    while (_window->IsOpen()) {
        while (const std::optional event = _window->GetWindow()->pollEvent())//si veo un evento, automaticamente se consume (un buffer)
        {
            eventHandler.HandleEvent(*event, *_window);
        }

        float deltaTime = deltaClock.restart().asSeconds();

        // Actualizaciones
        textField.Update(deltaTime);

        // Dibujar
        _window->GetWindow()->clear(sf::Color::Black);
        button.Render(*_window->GetWindow());
        button2.Render(*_window->GetWindow());
        textField.Render(*_window->GetWindow());
        _window->GetWindow()->display();
    }

    return 0;
}