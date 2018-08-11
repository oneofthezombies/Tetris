#pragma once

#include <Windows.h>
#include <bitset>

#include "vector2.h"

class Input
{
public:
    static void reset();
    static bool isOnceKeyDown(int key);
    static bool isOnceKeyUp(int key);
    static bool isStayKeyDown(int key);
    static bool isToggleKey(int key);
    static Vector2<> mousePosition();

private:
    static const int _kMaxSizeOfKey = 256;
    static std::bitset<_kMaxSizeOfKey> _key_up;
    static std::bitset<_kMaxSizeOfKey> _key_down;
};
