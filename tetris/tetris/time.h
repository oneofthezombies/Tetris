#pragma once

#include <chrono>

class Time
{
public:
    static void reset();
    static void update();
    static float deltaTime();

private:
    static std::chrono::system_clock::time_point _begin_time;
    static std::chrono::system_clock::time_point _previous_time;
    static float _delta_time;
};
