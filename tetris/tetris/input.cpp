#include "stdafx.h"
#include "input.h"

std::bitset<Input::_kMaxSizeOfKey> Input::_key_up;
std::bitset<Input::_kMaxSizeOfKey> Input::_key_down;

void Input::reset()
{
    _key_up.reset();
    _key_down.reset();
}

bool Input::isOnceKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        if (!_key_down[key])
        {
            _key_down.set(key);
            return true;
        }
    }
    else
        _key_down.reset(key);

    return false;
}

bool Input::isOnceKeyUp(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
        _key_up.set(key);
    else
        if (_key_up[key])
        {
            _key_up.reset(key);
            return true;
        }

    return false;
}

bool Input::isStayKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
        return true;

    return false;
}

bool Input::isToggleKey(int key)
{
    if (GetKeyState(key) & 0x0001)
        return true;

    return false;
}

Vector2<> Input::mousePosition()
{
    POINT pos;
    GetCursorPos(&pos);
    pos.x -= 108;
    pos.y -= 156;
    return Vector2<>(static_cast<float>(pos.x), static_cast<float>(pos.y));
}
