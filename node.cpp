#include <climits>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "node.h"

void Node::reset()
{
	parent = nullptr;
	moveCost = INT_MAX;
	totalCost = INT_MAX;
	isObstacle = false;
}

//calculate the cost to move from current node to goal (Manhattan distance)
int Node::heuristic(const V2i& goal) const
{
	const V2i delta = location - goal;
	const int manhattanDist = std::abs(delta.x) + std::abs(delta.y);
	return manhattanDist;
}

void Node::updateTotalCost(const V2i& goal)
{
	totalCost = moveCost + heuristic(goal);
}