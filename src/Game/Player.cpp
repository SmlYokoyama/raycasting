#include "Game/Player.hpp"
#include "Math/Physics.hpp"
#include "Math/Constants.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

namespace Game
{
    Player::Player(Math::Vector2D<float> pos, Game::Map* mp, float rad, float ang, float vel, float angVel)
        : position(pos), map(mp), radius(rad), angle(ang), velocity(vel), angularVelocity(angVel)
    {}

    Player::~Player()
    {}

    const Math::Vector2D<float>& Player::getPosition() const
    {
        return position;
    }

    void Player::setPosition(const Math::Vector2D<float>& pos)
    {
        position = pos;
    }

    void Player::normalizeAngle()
    {
        while(angle < 0.0f)
            angle += TWO_PI;
        while(angle >= TWO_PI)
            angle -= TWO_PI;
    }


    const float Player::getAngle() const
    {
        return angle;
    }

    void Player::setAngle(float ang)
    {
        angle = ang;
    }

    // TODO: add strafing (or not)
    void Player::handleInput()    // Keyboard input
    {
        // Since more than one key can be pressed at the same time,
        // we need to check each key separately
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            Math::Physics::updatePosition(position, velocity, angle, map, radius);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            Math::Physics::updatePosition(position, -velocity, angle, map, radius);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            Math::Physics::updateAngle(angle, -angularVelocity);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            Math::Physics::updateAngle(angle, angularVelocity);
    }
}