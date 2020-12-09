#pragma once

#include "vector.h"

class Node
{
public:
	V2i location;
	int moveCost = 0;
	int totalCost = 0;
	bool isObstacle = false;
	Node* parent = nullptr;

	void reset();
	int heuristic(const V2i& goal) const;
	void updateTotalCost(const V2i& goal);
};