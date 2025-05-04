#pragma once
#include <SFML/Graphics.hpp>

class FontManager {
public:
    static sf::Font& GetMainFont() 
    {
        static sf::Font font;
        static bool loaded = false;

        if (!loaded) 
        {
            font.openFromFile("Assets/Fonts/Poppins-Bold.ttf");
            loaded = true;
        }

        return font;
    }
};
