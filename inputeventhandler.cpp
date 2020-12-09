#include "inputeventhandler.h"
#include <cstdlib>
#include <conio.h>

InputEvent InpuEvetHandler::getKeyboardEvent()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 65:    // key up
            return InputEvent::Up;
            break;
        case 66:    // key down
            return InputEvent::Down;
            break;
        case 67:    // key right
            return InputEvent::Right;
            break;
        case 68:    // key left
            return InputEvent::Left;
            break;
        }
    }
}

std::unique_ptr<InpuEvetHandler> CreateInputEventHandler()
{
    return std::make_unique<InpuEvetHandler>();
}