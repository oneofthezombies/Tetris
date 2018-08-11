#pragma once

#include <Windows.h>

#include "component.h"
#include "vector2.h"

class Transform : public Component
{
public:
    Transform(GameObject* owner)
        : Component(owner)
        , _size(Vector2<>{})
        , _position(Vector2<>{})
        , _velocity(Vector2<>{})
    {
    }

    virtual ~Transform()
    {
    }

    void size(Vector2<> const& size)
    {
        _size = size;
    }

    void size(Vector2<>&& size)
    {
        _size = std::move(size);
    }

    void size(float const& x, float const& y)
    {
        _size.x = x;
        _size.y = y;
    }

    Vector2<> size() const
    {
        return _size;
    }

    template<class T>
    void position(T&& x, T&& y)
    {
        _position.x = static_cast<float>(std::forward<T>(x));
        _position.y = static_cast<float>(std::forward<T>(y));
    }

    void position(Vector2<> const& position)
    {
        _position = position;
    }

    void position(Vector2<>&& position)
    {
        _position = std::move(position);
    }

    void position(float const& x, float const& y)
    {
        _position.x = x;
        _position.y = y;
    }

    Vector2<> position() const
    {
        return _position;
    }

    void velocity(Vector2<> const& velocity)
    {
        _velocity = velocity;
    }

    void velocity(Vector2<>&& velocity)
    {
        _velocity = std::move(velocity);
    }

    void velocity(float const& x, float const& y)
    {
        _velocity.x = x;
        _velocity.y = y;
    }

    Vector2<> velocity() const
    {
        return _velocity;
    }

    void move()
    {
        _position += _velocity;
    }

    void move(float const& velocityX, float const& velocityY)
    {
        moveX(velocityX);
        moveY(velocityY);
    }

    void move(Vector2<> const& velocity)
    {
        _position += velocity;
    }

    void moveX(float const& velocityX)
    {
        _position.x += velocityX;
    }

    void moveX(float&& velocityX)
    {
        _position.x += std::move(velocityX);
    }

    void moveY(float const& velocityY)
    {
        _position.y += velocityY;
    }

    void moveY(float&& velocityY)
    {
        _position.y += std::move(velocityY);
    }

    Vector2<> globalPosition()
    {
        auto pos = _position;
        auto p = getOwner()->parent();
        while (p)
        {
            if (auto p_tr = p->getComponent<Transform>())
                pos += p_tr->position();

            p = p->parent();
        }

        return pos;
    }

    RECT toRect()
    {
        float half_width = _size.x / 2.0f;
        float half_height = _size.y / 2.0f;
        return RECT
        {
            static_cast<LONG>(_position.x - half_width), static_cast<LONG>(_position.y - half_height),
            static_cast<LONG>(_position.x + half_width), static_cast<LONG>(_position.y + half_height)
        };
    }

private:
    Vector2<> _size;
    Vector2<> _position;
    Vector2<> _velocity;
};
