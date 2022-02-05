#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Firework.hpp"

constexpr uint32_t WINDOW_WIDTH  = 1600;
constexpr uint32_t WINDOW_HEIGHT = 900;

constexpr float gravity = 600.0f;

std::random_device rgen;

void CreateBlastFireworks(std::vector<Firework>& blast, const Firework& parent);

int RandInt(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(rgen);
}

float Length(sf::Vector2f v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Normalize(sf::Vector2f v) {
    float length = Length(v);

    return sf::Vector2f(v.x / length, v.y / length);
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Fireworks!", sf::Style::Close, settings);
    window.setFramerateLimit(144);
    
    sf::Event event;

    sf::Clock deltaClock;

    float dt = 1.0f / 60.0f;
    
    sf::Shader fadeShader;
    fadeShader.loadFromFile("res/shaders/fade.vert", "res/shaders/fade.frag");

    sf::RenderTexture fadeTexture;
    fadeTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    float fadeTime = 0.00025f;

    sf::SoundBuffer buffer;
    sf::Sound       sound;
    buffer.loadFromFile("res/sounds/explosion.wav");
    sound.setBuffer(buffer);

    sf::CircleShape fireworkProp;

    std::vector<Firework> Fireworks;
    std::vector<Firework> BlastFireworks;

    bool MousePressed = false;

    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                window.close();
            }
        }


        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !MousePressed) {
            sf::Vector2f mousePos(sf::Mouse::getPosition(window));
            
            Fireworks.emplace_back
            (
                (float)RandInt(12, 16), 
                sf::Vector2f(mousePos.x, WINDOW_HEIGHT + 100.0f), 
                sf::Vector2f(0.0f, -1000.0f), 
                sf::Color(RandInt(100, 255), RandInt(100, 255), RandInt(100, 255))
            );

            MousePressed = true;
        }
        else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            MousePressed = false;

        window.clear();

        static float fadeElapsedTime = 0.0f;
        fadeElapsedTime += dt;

        if(fadeElapsedTime >= fadeTime) {
            fadeTexture.draw(sf::Sprite(fadeTexture.getTexture()), &fadeShader);
            fadeElapsedTime -= fadeTime;
        }
        
        for(size_t i = 0; i < Fireworks.size(); i++) {            
            auto& f = Fireworks[i];
            
            f.Velocity.y += gravity * dt;
            
            if(f.Velocity.y > 0.0f) {
                CreateBlastFireworks(BlastFireworks, f);

                sound.play();
                
                Fireworks.erase(Fireworks.begin() + i);
                i--;
                continue;
            }

            f.Position += f.Velocity * dt;
            
            fireworkProp.setRadius(f.Radius);
            fireworkProp.setFillColor(f.Color);
            fireworkProp.setOrigin(sf::Vector2f(f.Radius, f.Radius));
            fireworkProp.setPosition(f.Position);

            fadeTexture.draw(fireworkProp);
        }

        for(size_t i = 0; i < BlastFireworks.size(); i++) {            
            auto& f = BlastFireworks[i];
            
            f.Velocity.y += gravity * dt;
            
            f.Position += f.Velocity * dt;
            
            f.Radius -= 8.0f * dt;
            if(f.Radius <= 2.0f) {
                BlastFireworks.erase(BlastFireworks.begin() + i);
                i--;
                continue;
            }

            fireworkProp.setRadius(f.Radius);
            fireworkProp.setFillColor(f.Color);
            fireworkProp.setOrigin(sf::Vector2f(f.Radius, f.Radius));
            fireworkProp.setPosition(f.Position);

            fadeTexture.draw(fireworkProp);
        }
        
        fadeTexture.display();

        window.draw(sf::Sprite(fadeTexture.getTexture()));

        window.display();

        dt = deltaClock.restart().asSeconds();
        
        if(dt > 0.3f)
            dt = 1.0f / 60.0f;
    }

    return 0;
}

void CreateBlastFireworks(std::vector<Firework>& blast, const Firework& parent) {
    constexpr uint32_t count = 20;

    float ms = (float)RandInt(200, 450);

    float angle = 360.0f / (float)count;

    sf::Vector2f point(parent.Position.x, parent.Position.y - 1.0f);

    sf::Transform t;
    t.rotate(angle, parent.Position);

    for(uint32_t i = 1; i < count; i++) {        
        point = t.transformPoint(point);

        sf::Vector2f v = Normalize(parent.Position - point);

        Firework f;

        f.Color    = parent.Color;
        f.Radius   = RandInt(6, 12);
        f.Position = parent.Position;
        f.Velocity = v * ms;

        blast.push_back(f);
    }
}