#include "headers/TickCounter.hpp"
#include <SFML/System/Time.hpp>

rd::TickCounter& rd::TickCounter::update()
{
    if (clock.getElapsedTime().asSeconds() < 1.f) [[likely]]
    {
        ++current_ticks_count;
    }
    else [[unlikely]]
    {
        ticks_per_second = current_ticks_count;
        current_ticks_count = 0;
        clock.restart();
    }

    return *this;
}

std::size_t rd::TickCounter::getTps() noexcept
{
    return ticks_per_second;
}
