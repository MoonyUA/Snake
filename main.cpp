#include "snake.h"
#include "irenderer.h"
#include "events.h"
#include <chrono>
#include <iostream>
#include <conio.h>

bool gameUpdate(Snake& snake)
{
    if (!snake.dead)
    {
        if (_kbhit())
        {
            switch (_getch())
            {
            case 72:    // key up
                if(snake.getMovementMode() == GameMovementMode::Manual)
                    snake.handleInput(InputEvent::Up);
                break;
            case 80:    // key down
                if (snake.getMovementMode() == GameMovementMode::Manual)
                    snake.handleInput(InputEvent::Down);
                break;
            case 77:    // key right
                if (snake.getMovementMode() == GameMovementMode::Manual)
                    snake.handleInput(InputEvent::Right);
                break;
            case 75:    // key left
                if (snake.getMovementMode() == GameMovementMode::Manual)
                    snake.handleInput(InputEvent::Left);
                break;
            case 'a':
                snake.setMovementMove(GameMovementMode::Automatic);
                break;
            case 'm':
                snake.setMovementMove(GameMovementMode::Manual);
                break;
            }
        }

        snake.move();
        snake.checkCollision();

        return true;
    }

    return false;
}

int main(int argc, char* argv[])
{
    V2i startLocation(rand() % 29 + 1, rand() % 29 + 1);
    std::unique_ptr<Snake> game = std::make_unique<Snake>(1, startLocation, V2i(30, 30));
    std::unique_ptr<IRenderer> renderer = CreateRenderer();

    //constexpr const size_t targetFPS = game->getSnakeBody().size();
    size_t targetFPS;
    std::chrono::nanoseconds targetFrameDuration;
    while (true)
    {
        // make basic game loop with target fps
        auto t0 = std::chrono::high_resolution_clock::now();
        targetFPS = game->getSnakeBody().size() / 2 > 3 ? game->getSnakeBody().size() / 2 : 3;
        targetFrameDuration = std::chrono::nanoseconds(std::chrono::seconds(1)) / targetFPS;

        bool gameEnds = !gameUpdate(*game);
        renderer->Draw(*game);

        if (gameEnds)
		{
            system("cls");

            gameEnds = false;
			std::cout << "Snake: game over!" << std::endl;
            std::cout << "Would you like to start over? y/n" << std::endl;
            char answer;
            std::cin >> answer;
            if (answer == 'y')
                game->restart();
            else
                break;
        }

		while (std::chrono::high_resolution_clock::now() < t0 + targetFrameDuration);
    }

    return 0;
}