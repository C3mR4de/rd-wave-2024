#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "headers/TickCounter.hpp"

void render(sf::RenderWindow& window, std::atomic<rd::TickCounter>& tps_counter)
{
    const bool active = window.setActive();

    rd::TickCounter fps_counter;

    sf::Font font("/usr/share/fonts/TTF/UbuntuMonoNerdFont-Regular.ttf");
    sf::Text debug_info(font);

    sf::String fps_string = "FPS: ";
    sf::String tps_string = "TPS: ";

    while (window.isOpen())
    {
        fps_counter.update();
        
        debug_info.setString(fps_string + std::to_string(fps_counter.getTps()) + "\n" + tps_string + std::to_string(tps_counter.load().getTps()));
        
        window.clear();
        window.draw(debug_info);
        window.display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "RD Wave 2024", sf::State::Fullscreen);
    const bool deactive = window.setActive(false);
      
    const auto on_close = [&window](const sf::Event::Closed&)
    {
        window.close();
    };

    std::atomic<rd::TickCounter> tps_counter;
    std::jthread rendering_thread(&render, std::ref(window), std::ref(tps_counter));
    
    while (window.isOpen())
    {
        window.handleEvents(on_close);
        tps_counter.store(tps_counter.load().update());
    }
}

