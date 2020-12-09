#include "pathfinder.h"
#include "snake.h"

inline static size_t makeNodeIndex(V2i location, V2i mapSize)
{
	const size_t nodeIndex = location.y * mapSize.x + location.x;
	return nodeIndex;
}

inline static bool indexIsValid(size_t index, size_t elementsCount)
{
	return elementsCount > index && index >= 0;
}

//performs a linear search for node in set
static inline bool contains(const std::vector<Node*>& set, const Node* node)
{
	return std::find(std::begin(set), std::end(set), node) != std::end(set);
}

const Node* Pathfinder::getNode(const V2i& location) const
{
	const size_t nodeIndex = makeNodeIndex(location, snake->getMapSize());
	return indexIsValid(nodeIndex, grid.size()) ? &grid[nodeIndex] : nullptr;
}

Node* Pathfinder::getNode(const V2i& location)
{
	const Pathfinder* ConstThis = this;
	return const_cast<Node*>(ConstThis->getNode(location));
}

Pathfinder::Pathfinder(Snake* snake) : snake(snake), pathFound(false)
{
	const V2i& windowSize = snake->getMapSize();
	grid.resize(windowSize.x * windowSize.y);
	for (int y = 0; y < windowSize.y; y++)
	{
		for (int x = 0; x < windowSize.x; x++)
		{
			getNode(V2i(x, y))->location = V2i(x, y);
		}
	}
}

template<typename Visitor>
inline void forEachNeighbor(const std::vector<Node>& nodes, V2i mapSize, Node* node, Visitor&& visitor)
{
	//will contain the VALID adjacent nodes to the argument node

	/* for each potential neighbor:
	* -check if it's in bounds
	* -check if it's NOT an obstacle
	* -if the potential neighbor does not have a parent, set it to the argument
	*  node */

	auto handleDirection = [&](const V2i& direction)
	{
		const size_t nodeIndex = makeNodeIndex(node->location + direction, mapSize);
		const Node* neighbor = indexIsValid(nodeIndex, nodes.size()) ? &nodes[nodeIndex] : nullptr;
		if (neighbor && (!neighbor->isObstacle))
		{
			visitor(nodeIndex);
		}
	};

	handleDirection(V2i(0, -1));
	handleDirection(V2i(0, +1));
	handleDirection(V2i(-1, 0));
	handleDirection(V2i(+1, 0));
}

void Pathfinder::AStar(const V2i& start, const V2i& goal, std::vector<InputEvent>& outBuffer)
{
	//fetch the most recent iteration of the game board
	updateGameState();

	visitQueue.clear();
	closedSet.clear();

	//init start node and add to open set
	{
		Node* startNode = getNode(start);
		visitQueue.push_back(startNode);
		startNode->moveCost = 0;
		startNode->totalCost = startNode->heuristic(goal);
	}

	while (!visitQueue.empty())
	{
		//choose the node in the open set w/ the lowest total cost
		std::sort(visitQueue.begin(), visitQueue.end(), [](const Node* nodeA, const Node* nodeB)
		{
			return (nodeA->totalCost > nodeB->totalCost);
		});
		Node* current = visitQueue.back();

		//don't do anymore path finding if we've found the goal!
		if (current->location == goal)
		{
			getNode(start)->parent = nullptr; //shitty spaghetti code
			buildPath(getNode(goal), outBuffer);

			//tell the game to stop path finding (we're done until we eat the food)
			pathFound = true;
			return;
		}

		//add the current node to the closed set and remove from open set
		visitQueue.pop_back();
		closedSet.push_back(current);

		//fetch the four adjacent nodes to the current node (if they are valid)
		forEachNeighbor(grid, snake->getMapSize(), current, [&](size_t neighborIndex)
		{
			Node* neighbor = &grid[neighborIndex];

			if (!neighbor->parent)
			{
				neighbor->parent = current;
				neighbor->moveCost = current->moveCost + 1;
			}

			if (!contains(closedSet, neighbor))
			{
				//update score 
				neighbor->updateTotalCost(goal);

				if (!contains(visitQueue, neighbor))
				{
					visitQueue.push_back(neighbor);
				}
			}
		});
	}

	pathFound = false;
}

void Pathfinder::buildPath(Node* goal, std::vector<InputEvent>& pathBuffer) const
{
	pathBuffer.clear();

	//Start from goal node, move backwards to start
	Node* current = goal;
	int pathLength = 0;

	while (current && current->parent)
	{
		Node* parent = current->parent;

		//parent = closer along path towards goal than current
		if (current->location.x < parent->location.x)
		{
			pathBuffer.push_back(InputEvent::Left);
		}
		else if (current->location.x > parent->location.x)
		{
			pathBuffer.push_back(InputEvent::Right);
		}
		else if (current->location.y < parent->location.y)
		{
			pathBuffer.push_back(InputEvent::Up);
		}
		else
		{
			pathBuffer.push_back(InputEvent::Down);
		}

		pathLength++;

		//traverse the path by moving to the next successor node
		current = parent;
	}
}

void Pathfinder::updateGameState()
{
	//initialize all grid cells to be empty

	//initialize game state vector
	const V2i& windowSize = snake->getMapSize();
	for (int i = 0; i < windowSize.y; i++)
	{
		for (int j = 0; j < windowSize.x; j++)
		{
			getNode(V2i(i, j))->reset();
		}
	}

	//set the snake's body as obstacles (ignore the head)
	for (int i = 1; i < snake->getSnakeBody().size(); i++)
	{
		getNode(snake->getSnakeBody()[i])->isObstacle = true;
	}

	for (int i = 0; i < snake->getWalls().size(); i++)
	{
		getNode(snake->getWalls()[i])->isObstacle = true;
	}
}

void Pathfinder::setRepeatSearch(bool repeat)
{
	repeatSearch = repeat;
}