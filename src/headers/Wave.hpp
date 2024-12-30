#ifndef WAVE_HPP
#define WAVE_HPP

#include <cstddef>
#include <cmath>
#include <array>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Angle.hpp>

namespace rd
{
    template <std::size_t trail_length = 200>
    struct Wave
    {
        Wave() = delete;

        struct State
        {
            enum Gravity: short int
            {
                Reversed = -1,
                Normal = 1
            };

            sf::Vector2f position;
            sf::Vector2f speed;
            Gravity gravity;
            sf::Angle angle;
            float thickness;
            std::array<sf::Vector2f, trail_length> trail;

            State& update(float dt, bool is_key_pressed);
        };

        struct Entity: public sf::Drawable
        {
            Entity(const sf::Texture& texture);
            Entity& update(const State& state);
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

            sf::Sprite sprite;
            sf::VertexArray trail = sf::VertexArray(sf::PrimitiveType::TriangleStrip, trail_length);
        };
    };
}

template <std::size_t trail_length>
rd::Wave<trail_length>::State& rd::Wave<trail_length>::State::update(float dt, bool is_key_pressed)
{
    angle = sf::degrees(45.f * static_cast<float>(gravity) * (is_key_pressed ? 1.f : -1.f));
    speed.y = speed.x * std::tan(angle.asRadians());
    position += speed * dt;

    for (std::size_t i = trail_length - 1; i > 1; --i)
    {
        trail[i] = trail[i - 2];
    }

    trail[0] = {position.x, position.y - thickness / 2.f};
    trail[1] = {position.x, position.y + thickness / 2.f};

    return *this;
}

template <std::size_t trail_length>
rd::Wave<trail_length>::Entity::Entity(const sf::Texture& texture):
    sprite(texture)
{
    sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
}

template <std::size_t trail_length>
rd::Wave<trail_length>::Entity& rd::Wave<trail_length>::Entity::update(const State& state)
{
    sprite.setPosition(state.position);
    sprite.setRotation(state.angle);
    
    for (std::size_t i = 0; i < trail_length; ++i)
    {
        trail[i] = {state.trail[i], sf::Color::Green};
    }

    return *this;
}

template <std::size_t trail_length>
void rd::Wave<trail_length>::Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(trail, states);
    target.draw(sprite, states);
}

#endif

