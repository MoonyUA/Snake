#include "snake.h"
#include "pathfinder.h"

void Snake::moveFood()
{
	score += 10;
	do
	{
		foodLocation.x = rand() % mapSize.x;
		foodLocation.y = rand() % mapSize.y;
	} while (std::find(std::begin(body), std::end(body), foodLocation) != std::end(body) || std::find(std::begin(walls), std::end(walls), foodLocation) != std::end(walls));
}

void Snake::initWalls()
{
	for (int i = 0; i < mapSize.x; ++i)
		for (int j = 0; j < mapSize.y; ++j)
		{
			if(i == 0 || (i == mapSize.x - 1) || j == 0 || (j == mapSize.y - 1))
				walls.push_back(V2i(i, j));
		}
}

Snake::Snake(int numSegments, V2i startLocation, V2i inMapSize)
	: maxBufferSize(1000)
	, direction(InputEvent::Left)
	, dead(false)
	, mapSize(inMapSize)
	, movementMode(GameMovementMode::Manual)
{
	pathfinder = std::make_unique<Pathfinder>(this);
	
	for (int i = 0; i < numSegments; i++)
	{
		body.push_back(startLocation + V2i(i, 0));
	}

	moveFood();
	initWalls();

	score = 0;
}

void Snake::restart()
{
	dead = false;
	body.clear();
	body.push_back(V2i(rand() % mapSize.x - 1, rand() % mapSize.y - 1));

	moveFood();
	initWalls();

	score = 0;
}

Snake::~Snake() = default;

bool Snake::checkDead()
{
	return dead;
}

void Snake::checkCollision()
{
	//check for snake eating itself
	if (std::find(body.begin()+1, body.end(), body.front()) != body.end())
	{
		dead = true;
	}

	if (std::find(walls.begin(), walls.end(), body.front()) != walls.end())
	{
		dead = true;
	}

	if (body.front() == foodLocation)
	{
		moveFood();
		pathfinder->pathFound = false;
		body.push_back(body.back());
	}

	//check for out of bounds
	if (!(body.front() >= V2i::Zero() && body.front() < mapSize))
	{

		dead = true;
	}
}

void Snake::move()
{
	if (movementMode == GameMovementMode::Automatic)
	{
		//only run A* if a valid path has not already been found
		if (!pathfinder->pathFound || pathfinder->repeatSearch)
		{
			pathfinder->AStar(body.front(), foodLocation, inputBuffer);
		}

		if (!inputBuffer.empty())
		{
			//read direction from the input buffer
			direction = inputBuffer.back();
			inputBuffer.pop_back();
		}
	}

	V2i newHead = body.front();
	body.pop_back();

	//move head based on current direction
	if (direction == InputEvent::Up)
		newHead += V2i(0, -1);
	else if (direction == InputEvent::Down)
		newHead += V2i(0, +1);
	else if (direction == InputEvent::Left)
		newHead += V2i(-1, 0);
	else
		newHead += V2i(1, 0);

	body.insert(body.begin(), newHead);
}

void Snake::handleInput(InputEvent inputEvent)
{
	switch (inputEvent)
	{
	case InputEvent::Up:
		if (direction != InputEvent::Down)
		{
			direction = InputEvent::Up; //add player input to input buffer
		}
		break;

	case InputEvent::Down:
		if (direction != InputEvent::Up)
		{
			direction = InputEvent::Down;
		}
		break;

	case InputEvent::Left:
		if (direction != InputEvent::Right)
		{
			direction = InputEvent::Left;
		}
		break;

	case InputEvent::Right:
		if (direction != InputEvent::Left)
		{
			direction = InputEvent::Right;
		}
		break;
	}
}

void Snake::setMovementMove(GameMovementMode mode)
{
	switch (mode)
	{
	case GameMovementMode::Automatic:
		movementMode = GameMovementMode::Automatic;
		break;
	case GameMovementMode::Manual:
		movementMode = GameMovementMode::Manual;
		break;
	}
}