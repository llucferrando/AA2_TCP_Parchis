#include "BoardPositions.h"

// -- ALL THE POSITIONS TO BE USED IN GAMEPLAY

int GetEntryToGoalIndex(PlayerColor color) 
{
    switch (color) 
    {
        case PlayerColor::RED: 
            return 30;
        case PlayerColor::BLUE: 
         return 15;
        case PlayerColor::GREEN: 
            return 45;
        case PlayerColor::YELLOW: 
            return 60;
        default: 
            return 0;
    }
}

std::vector<sf::Vector2f> mainPathPositions = 
{
    {436, 673}, {436, 637}, {436, 600}, {436, 560}, {436, 526}, {436, 489}, {436, 453},
    {457, 432}, {494, 432}, {530, 432}, {568, 432}, {604, 432}, {641, 432}, {677, 432},
    {677, 351}, 
    {677, 277}, {642, 277}, {605, 277}, {568, 277}, {531, 277}, {495, 277}, {459, 277},
    {436, 258}, {436, 221}, {436, 186}, {436, 148}, {436, 112}, {436, 77}, {436, 39},
    {360, 39},
    {281, 39}, {281, 74}, {281, 109}, {281, 147}, {281, 185}, {281, 219}, {281, 258},
    {260, 277}, {224, 277}, {188, 277}, {151, 277}, {114, 277}, {76, 277}, {41, 277},
    {41, 360},
    {41, 432}, {76, 432}, {113, 432}, {150, 432}, {186, 432}, {222, 432}, {260, 432},
    {281, 451}, {281, 488}, {281, 525}, {281, 560}, {281, 598}, {281, 635}, {281, 670},
    {360, 670},
};

std::map<PlayerColor, sf::Vector2f> startPositions = 
{
    {PlayerColor::RED, {280, 109}},
    {PlayerColor::GREEN, {150, 433}},
    {PlayerColor::BLUE, {560, 278}},
    {PlayerColor::YELLOW, {436,560}}    
};

std::map<PlayerColor, std::vector<sf::Vector2f>> metaPositions = 
{
    {PlayerColor::RED, {{360,76}, {360,114}, {360,150}, {360,186}, {360,224}, {360,261}, {360,309}}},
    {PlayerColor::GREEN, {{76,360}, {114,360}, {150,360}, {186,360}, {224,360}, {261,360}, {309,360}}},
    {PlayerColor::BLUE, {{640,360}, {604,360}, {568,360}, {530,360}, {495,360}, {459,360}, {409,360}}},
    {PlayerColor::YELLOW, {{360,635}, {360,598}, {360,562}, {360,525}, {360,490}, {360,452}, {360,404}}}
};

std::map<PlayerColor, std::vector<sf::Vector2f>> homePositions = 
{
    {PlayerColor::RED, {{70,75}, {70, 195}, {177,75}, {177, 195}}},
    {PlayerColor::GREEN, {{70,523}, {70,640}, {177,523}, {177,640}}},
    {PlayerColor::BLUE, {{540,75}, {650,75}, {540,195}, {650,195}}},
    {PlayerColor::YELLOW, {{540,523}, {650,523}, {540,640}, {650,640}}},    
};