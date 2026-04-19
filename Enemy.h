#pragma once
#include <SFML/Graphics.hpp> // Add this include for Sprite and Texture
#include "Global.h"

class Enemy
{
public:
    int x, y;
    int dx, dy;
    float speed;
    float baseSpeed;
    sf::Sprite sprite; // Add a sprite member to the Enemy class
    sf::Texture texture; // Add a texture to load the sprite image

    Enemy()
    {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
        baseSpeed = 4.0f;
        speed = baseSpeed;

        // Load a texture and set it to the sprite
        if (!texture.loadFromFile("images/enemy.png")) // Replace "enemy.png" with your texture file
        {
            // Handle texture loading failure
            std::cerr << "Failed to load enemy texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
    }

    void move()
    {
        x += static_cast<int>(dx * speed / baseSpeed);
        if (grid[y / ts][x / ts] == 1) {
            dx = -dx;
            x += static_cast<int>(dx * speed / baseSpeed);
        }
        y += static_cast<int>(dy * speed / baseSpeed);
        if (grid[y / ts][x / ts] == 1) {
            dy = -dy;
            y += static_cast<int>(dy * speed / baseSpeed);
        }
        sprite.setPosition(static_cast<float>(x), static_cast<float>(y)); // Update sprite position
    }

    void setSpeed(float newSpeed)
    {
        speed = newSpeed;
    }

    void resetSpeed()
    {
        speed = baseSpeed;
    }

    // Getters for x, y, and sprite
    int getX() const { return x; }
    int getY() const { return y; }
    const sf::Sprite& getSprite() const { return sprite; } // Add getSprite method
};