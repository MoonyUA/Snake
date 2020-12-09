#pragma once

#include <memory>

class Snake;

class IRenderer
{
public:
	virtual void Draw(const Snake&) = 0;
	virtual ~IRenderer() = default;
};

std::unique_ptr<IRenderer> CreateRenderer();
