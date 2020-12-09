#pragma once

#include <memory>
#include <vector>
#include <string>

#include "vector.h"
#include "events.h"

class Pathfinder;

class Snake
{
private:
	std::unique_ptr<Pathfinder> pathfinder;

	std::vector<V2i> body;
	std::vector<V2i> walls;
	std::vector<InputEvent> inputBuffer;

	size_t maxBufferSize;

	GameMovementMode movementMode;
	InputEvent direction;		//The snake's current heading (change to enum?)
	int score;

	V2i foodLocation;
	V2i mapSize;

	void moveFood();
	void initWalls();

public:
	Snake(int numSegments, V2i startLocation, V2i mapSize);
	~Snake();

	void handleInput(InputEvent);
	void setMovementMove(GameMovementMode);

	bool checkDead();					  //return game state

	void move();						  //Step the snake in the current direction
	void restart();
	void bufferInput();
	void checkCollision(); /*check if the snake has:
							 hit itself
							 hit a boundary
							 hit the food*/

	inline const V2i& getMapSize() const { return mapSize; }
	inline const V2i& getFoodLocation() const { return foodLocation; }
	inline const std::vector<V2i>& getSnakeBody() const { return body; }
	inline const std::vector<V2i>& getWalls() const { return walls; }
	inline const int& getScore() const { return score; }
	inline const GameMovementMode& getMovementMode() const { return movementMode; }

	bool dead = false;
};