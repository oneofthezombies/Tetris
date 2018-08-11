#pragma once

#include <functional>

#include "time.h"

class CoolTimer
{
public:
    CoolTimer(float const& time, std::function<void()> const& operation = std::bind(std::function<void()> {}), int const& count = 0x7FFFFFFF)
        : _operation(operation)
        , _cool_time(time)
        , _cool_down(_cool_time)
        , _count(count)
        , _work(std::bind(&CoolTimer::work, this))
    {
    }

    bool operator()()
    {
        return _work();
    }

private:
    bool work()
    {
        if (_count <= 0)
        {
            _work = std::bind(&CoolTimer::doNothing, this);
            return false;
        }

        _cool_down -= Time::deltaTime();
        if (_cool_down <= 0.0f)
        {
            if (_count < 0x7FFFFFFF)
                _count--;

            _cool_down = _cool_time;
            _operation();
            return true;
        }

        return false;
    }

    bool doNothing()
    {
        // do nothing
        return false;
    }

    const float _cool_time;
    float _cool_down;
    int _count;
    std::function<void()> _operation;
    std::function<bool()> _work;
};
