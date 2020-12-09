#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "events.h"

class InpuEvetHandler
{
public:
	InputEvent getKeyboardEvent();
};

std::unique_ptr<InpuEvetHandler> CreateInputEventHandler();