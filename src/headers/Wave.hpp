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
                Reversed = 1,
                Normal = -1
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
            sf::VertexArray trail = sf::VertexArray(sf::PrimitiveType::TriangleStrip, trail_length * 2);
        };
    };
}

template <std::size_t trail_length>
rd::Wave<trail_length>::State& rd::Wave<trail_length>::State::update(float dt, bool is_key_pressed)
{
    angle = sf::degrees(std::abs(angle.asDegrees()) * static_cast<float>(gravity) * (is_key_pressed ? 1.f : -1.f));
    speed.y = speed.x * std::tan(angle.asRadians());
    position += speed * dt;

    for (std::size_t i = trail_length - 1; i > 0; --i)
    {
        trail[i] = trail[i - 1];
    }

    trail[0] = position;

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
        const auto [x, y] = state.trail[i];
        trail[2 * i] = {{x, y - state.thickness / 2.f}, sf::Color::Green};
        trail[2 * i + 1] = {{x, y + state.thickness + 2.f}, sf::Color::Green};
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

