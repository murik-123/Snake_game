#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>
#include <vector>
#include <string>

int main(){
    constexpr int windowWidth = 640;
    constexpr int windowHeight = 480;
    constexpr int cellSize = 40;
    constexpr int columns = windowWidth / cellSize;
    constexpr int rows = windowHeight / cellSize;

    sf::RenderWindow window(
        sf::VideoMode({windowWidth, windowHeight}),
        "Snake - Score: 0"
    );
    window.setKeyRepeatEnabled(false);

    std::vector<sf::Vector2i> snake = {
        {5, 5},
        {4, 5},
        {3, 5}
    };

    sf::Vector2i direction{1, 0};
    std::vector<sf::Vector2i> apples;

    std::mt19937 randomEngine(std::random_device{}());
    std::uniform_int_distribution<int> randomColumn(0, columns - 1);
    std::uniform_int_distribution<int> randomRow(0, rows - 1);

    int score = 0;

    auto addApples = [&]()
    {
        while (apples.size() < 2 &&
               snake.size() + apples.size() < columns * rows)
        {
            sf::Vector2i position{
                randomColumn(randomEngine),
                randomRow(randomEngine)
            };

            const bool onSnake =
                std::ranges::find(snake, position) != snake.end();

            const bool onApple =
                std::ranges::find(apples, position) != apples.end();

            if (!onSnake && !onApple)
                apples.push_back(position);
        }
    };

    addApples();

    sf::RectangleShape segment({34.f, 34.f});
    segment.setFillColor(sf::Color::Green);

    sf::CircleShape appleShape(8.f);
    appleShape.setFillColor(sf::Color::Red);

    sf::RectangleShape verticalLine({1.f, static_cast<float>(windowHeight)});
    sf::RectangleShape horizontalLine({static_cast<float>(windowWidth), 1.f});
    verticalLine.setFillColor(sf::Color(75, 75, 85));
    horizontalLine.setFillColor(sf::Color(75, 75, 85));

    sf::Clock clock;
    float timeSinceMove = 0.f;
    bool gameOver = false;

    while (window.isOpen())
    {
        constexpr float moveInterval = 0.15f;
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (!gameOver)
            {
                if (const auto* key = event->getIf<sf::Event::KeyPressed>())
                {
                    switch (key->code)
                    {
                        case sf::Keyboard::Key::W:
                            if (direction.y != 1)
                                direction = {0, -1};
                            break;

                        case sf::Keyboard::Key::S:
                            if (direction.y != -1)
                                direction = {0, 1};
                            break;

                        case sf::Keyboard::Key::A:
                            if (direction.x != 1)
                                direction = {-1, 0};
                            break;

                        case sf::Keyboard::Key::D:
                            if (direction.x != -1)
                                direction = {1, 0};
                            break;

                        default:
                            break;
                    }
                }
            }
        }

        timeSinceMove += clock.restart().asSeconds();

        while (timeSinceMove >= moveInterval && !gameOver)
        {
            timeSinceMove -= moveInterval;

            sf::Vector2i nextHead = snake.front() + direction;

            if (nextHead.x < 0 || nextHead.x >= columns ||
                nextHead.y < 0 || nextHead.y >= rows)
            {
                gameOver = true;
                window.setTitle("Game over - close the window to exit");
                break;
            }

            auto apple = std::ranges::find(apples, nextHead);
            bool eatingApple = apple != apples.end();

            std::size_t bodyPartsToCheck = snake.size();
            if (!eatingApple)
                --bodyPartsToCheck;

            bool hitSelf = false;
            for (std::size_t i = 0; i < bodyPartsToCheck; ++i)
            {
                if (snake[i] == nextHead)
                {
                    hitSelf = true;
                    break;
                }
            }

            if (hitSelf)
            {
                gameOver = true;
                window.setTitle("Game over - close the window to exit");
                break;
            }

            snake.insert(snake.begin(), nextHead);

            if (eatingApple)
            {
                apples.erase(apple);
                ++score;
                window.setTitle("Snake - Score: " + std::to_string(score));
                addApples();
            }else{
                snake.pop_back();
            }
        }

        window.clear(sf::Color(30, 30, 40));

        for (int x = 0; x <= windowWidth; x += cellSize)
        {
            verticalLine.setPosition({static_cast<float>(x), 0.f});
            window.draw(verticalLine);
        }

        for (int y = 0; y <= windowHeight; y += cellSize)
        {
            horizontalLine.setPosition({0.f, static_cast<float>(y)});
            window.draw(horizontalLine);
        }

        for (const sf::Vector2i& position : apples)
        {
            appleShape.setPosition({
                static_cast<float>(position.x * cellSize + 12),
                static_cast<float>(position.y * cellSize + 12)
            });
            window.draw(appleShape);
        }

        for (const sf::Vector2i& position : snake)
        {
            segment.setPosition({
                static_cast<float>(position.x * cellSize + 3),
                static_cast<float>(position.y * cellSize + 3)
            });
            window.draw(segment);
        }
        window.display();
    }
}