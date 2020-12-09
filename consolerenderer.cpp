#define NOMINMAX
#include "Windows.h"
#include "irenderer.h"
#include "snake.h"
#include <cassert>
#include <algorithm>
#include <string>


class ConsoleRenderer : public IRenderer
{

public:
	void Draw(const Snake& snake) override
	{
		const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		assert(consoleHandle != INVALID_HANDLE_VALUE);

		CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo);

		const V2i mapSize = snake.getMapSize();
		V2i bufferSize(bufferInfo.dwSize.X, bufferInfo.dwSize.Y);
		bufferSize.x = std::max(mapSize.x, bufferSize.x);
		bufferSize.y = std::max(mapSize.y, bufferSize.y);
		mapBuffer.clear();
		mapBuffer.resize(bufferSize.x * bufferSize.y, ' ');

		auto setCharAt = [&](const V2i pos, char c)
		{
			const int index = bufferSize.x * pos.y + pos.x;
			if (index >= 0 && index < static_cast<int>(mapBuffer.size()))
			{
				mapBuffer[index] = c;
			}
		};

		setCharAt(snake.getFoodLocation(), '@');
		
		const std::vector<V2i> body = snake.getSnakeBody();
		setCharAt(body[0], 'O');
		for (size_t i = 1; i < body.size(); ++i)
		{
			setCharAt(body[i], 'X');
		}

		const std::vector<V2i> walls = snake.getWalls();
		for (size_t i = 0; i < walls.size(); ++i)
		{
			setCharAt(walls[i], '#');
		}

		for (size_t i = 0; i < mapSize.y; ++i)
		{
			setCharAt(V2i(mapSize.x+2,i), '|');
		}

		int score = snake.getScore();
		std::string s = "Score: " + std::to_string(score);
		for (size_t i = 0; i < s.size(); ++i)
		{
			setCharAt(V2i(mapSize.x + 5 + i, 0), s.at(i));
		}

		s = "Press 'a' to activate auto movement";
		for (size_t i = 0; i < s.size(); ++i)
		{
			setCharAt(V2i(mapSize.x + 5 + i, 3), s.at(i));
		}

		s = "Press 'm' to deactivate auto movement";
		for (size_t i = 0; i < s.size(); ++i)
		{
			setCharAt(V2i(mapSize.x + 5 + i, 4), s.at(i));
		}

		DWORD written;
		WriteConsoleOutputCharacterA(consoleHandle, mapBuffer.data(), (DWORD)mapBuffer.size(), COORD{ 0, 0 }, &written);
	}

private:
	std::vector<char> mapBuffer;

};

std::unique_ptr<IRenderer> CreateRenderer()
{
	return std::make_unique<ConsoleRenderer>();
}