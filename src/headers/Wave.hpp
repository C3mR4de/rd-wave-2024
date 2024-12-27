#ifndef WAVE_HPP
#define WAVE_HPP

#include <SFML/Graphics/Sprite.hpp>

namespace rd
{
    class Wave
    {
    public:

        Wave(const sf::Texture& texture);
        Wave(sf::Texture&& texture) = delete;
        Wave& operator=(const sf::Texture& texture);
        Wave& operator=(sf::Texture&& texture) = delete;     
        ~Wave() noexcept = default;

        Wave& update();        

    private:

        sf::Sprite sprite;
    };
}

#endif

