#include <cstddef>
#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "headers/TickCounter.hpp"
#include "headers/Wave.hpp"

constexpr std::size_t trail_length = 40000;
using Wave = rd::Wave<trail_length>;

void render(sf::RenderWindow& window, std::atomic<rd::TickCounter>& tps_counter, std::atomic<Wave::State>& wave_state)
{
    const bool active = window.setActive();

    rd::TickCounter fps_counter;

    sf::Font font("/usr/share/fonts/TTF/UbuntuMonoNerdFont-Regular.ttf");
    sf::Text debug_info(font);

    sf::String fps_string = "FPS: ";
    sf::String tps_string = "TPS: ";

    sf::Texture texture("wave_head.png");
    Wave::Entity wave_entity(texture);

    sf::View view(window.getDefaultView());

    window.setFramerateLimit(5);

    while (window.isOpen())
    {
        wave_entity.update(wave_state.load());
        view.setCenter({wave_entity.sprite.getPosition().x + 200.f, view.getCenter().y});
        window.setView(view);
        
        fps_counter.update();
        debug_info.setString(fps_string + std::to_string(fps_counter.getTps()) + "\n" + tps_string + std::to_string(tps_counter.load().getTps()));
        debug_info.setPosition(view.getCenter() - view.getSize() / 2.f);

        window.clear();
        window.draw(wave_entity);
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
    const auto [x, y] = window.getDefaultView().getCenter();
    std::atomic<Wave::State> wave_state(Wave::State{{0, x}, {1000, 0}, Wave::State::Gravity::Normal, sf::degrees(45.f), 30.f});
    std::jthread rendering_thread(&render, std::ref(window), std::ref(tps_counter), std::ref(wave_state));

    sf::Clock frame_clock;

    while (window.isOpen())
    {
        float dt = frame_clock.restart().asSeconds();
        window.handleEvents(on_close);

        tps_counter.store(tps_counter.load().update());
        wave_state.store(wave_state.load().update(dt, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)));

        // using namespace std::chrono_literals;
        // std::this_thread::sleep_for(2ms);
    }
}

