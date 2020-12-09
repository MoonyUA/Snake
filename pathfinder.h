#pragma once

#include <climits>
#include <vector>
#include <algorithm>
#include <ctime>
#include <numeric>

#include "node.h"
#include "events.h"

//forward declarations
class Snake;

class Pathfinder
{
private:
	Snake* snake;
	std::vector<Node> grid;

	// cached sets
	std::vector<Node*> visitQueue;
	std::vector<Node*> closedSet;

	Node* getNode(const V2i& location);
	const Node* getNode(const V2i& location) const;

public:
	Pathfinder(Snake* snake);

	//Walks the path created by A*, returns a buffer of directions
	void buildPath(Node* goal, std::vector<InputEvent>& outBuffer) const;
	void setRepeatSearch(bool repeat);
	void AStar(const V2i& start, const V2i& goal, std::vector<InputEvent>& outBuffer);
	void updateGameState();

	bool pathFound;
	bool repeatSearch;
};
