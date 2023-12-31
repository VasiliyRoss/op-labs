#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <cmath>

struct Object
{
    sf::Sprite sprite;
    sf::Texture texture;
};

void initObject(Object &object, sf::Vector2f &position, std::string pathToTexture);
void pollEvents(sf::RenderWindow &window, sf::Vector2f &mouseClickPosition);
void redrawFrame(sf::RenderWindow &window, Object &object1, Object &object2);
void updateObjectPositionOnClick(Object &object, sf::Vector2f &position);
void updateMovingObject(Object &object, sf::Vector2f &targetPosition, sf::Clock &clock);
sf::Vector2f toEuclidean(float radius, float angle);
float toDegrees(float radians);

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Kitty follows laser", sf::Style::Default, settings);

    sf::Vector2f startPosition = {400, 300};
    sf::Vector2f mouseClickPosition = startPosition;
    sf::Clock clock;

    Object cat;
    Object lasertPointer;

    initObject(lasertPointer, startPosition, "assets/red_pointer.png");
    initObject(cat, startPosition, "assets/cat.png");

    while (window.isOpen())
    {
        pollEvents(window, mouseClickPosition);
        updateObjectPositionOnClick(lasertPointer, mouseClickPosition);
        updateMovingObject(cat, mouseClickPosition, clock);
        redrawFrame(window, lasertPointer, cat);
    }
}

void initObject(Object &object, sf::Vector2f &position, std::string pathToTexture)
{
    if (!object.texture.loadFromFile(pathToTexture))
    {
        std::cout << "Failed to load image" << std::endl;
        return;
    }

    object.sprite.setTexture(object.texture);
    object.sprite.setOrigin(sf::Vector2f(object.sprite.getTexture()->getSize().x * 0.5, object.sprite.getTexture()->getSize().y * 0.5));
    object.sprite.setPosition(position);
}

void pollEvents(sf::RenderWindow &window, sf::Vector2f &mouseClickPosition)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                mouseClickPosition = {
                    float(event.mouseButton.x),
                    float(event.mouseButton.y)};
            }
        default:
            break;
        }
    }
}

void redrawFrame(sf::RenderWindow &window, Object &object1, Object &object2)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    window.draw(object1.sprite);
    window.draw(object2.sprite);
    window.display();
}

void updateObjectPositionOnClick(Object &object, sf::Vector2f &position)
{
    object.sprite.setPosition(position);
}

float toDegrees(float radians)
{
    return float(double(radians) * 180 / M_PI);
}

sf::Vector2f toEuclidean(float radius, float angle)
{
    return {
        radius * std::cos(angle),
        radius * std::sin(angle)};
}

void updateMovingObject(Object &object, sf::Vector2f &targetPosition, sf::Clock &clock)
{
    constexpr float movementSpeed = 200.f;
    const float deltaTime = clock.restart().asSeconds();
    const sf::Vector2f currentPosition = object.sprite.getPosition();

    if (currentPosition != targetPosition)
    {
        const sf::Vector2f motion = targetPosition - currentPosition;
        const float vectorLength = std::sqrt(motion.x * motion.x + motion.y * motion.y);
        const sf::Vector2f direction = {
            motion.x / vectorLength,
            motion.y / vectorLength};
        const float distance = movementSpeed * deltaTime;
        object.sprite.setPosition(currentPosition + direction * distance);
    }

    if (currentPosition.x > targetPosition.x)
        object.sprite.setScale(-1, 1);
    else
        object.sprite.setScale(1, 1);
}