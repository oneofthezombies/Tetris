#pragma once

#include "singleton.h"

class _ImageManager : public Singleton<_ImageManager>
{
public:


private:

};

struct ImageManager
{
    _ImageManager* operator()()
    {
        return _ImageManager::getInstance();
    }
};