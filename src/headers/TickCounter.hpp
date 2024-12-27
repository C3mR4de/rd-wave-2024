#ifndef TICK_COUNTER_HPP
#define TICK_COUNTER_HPP

#include <cstddef>
#include <SFML/System/Clock.hpp>

namespace rd
{
    class TickCounter
    {
    public:

        TickCounter& update();
        std::size_t getTps() noexcept;

    private:

        sf::Clock clock;
        std::size_t current_ticks_count;
        std::size_t ticks_per_second;
    };
}

#endif

