#pragma once

class GameObject;

class Component
{
public:
    Component(GameObject* owner) 
        : _owner(owner)
    {
    }

    virtual ~Component() 
    {
    }

    virtual GameObject* getOwner() const final
    {
        return _owner;
    }

private:
    GameObject* _owner;
};
