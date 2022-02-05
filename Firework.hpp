#pragma once

#include <SFML/Graphics.hpp>

struct Firework 
{
    Firework() = default;
    
    Firework(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
        : Radius(radius)
        , Position(position)
        , Velocity(velocity)
        , Color(color)
    {

    }
    
    float        Radius = 0.0f;
    sf::Vector2f Position;
    sf::Vector2f Velocity;
    sf::Color    Color;
};