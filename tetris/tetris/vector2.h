#pragma once

#include <utility>

template<class T = float>
struct Vector2
{
    Vector2()
        : x(0)
        , y(0)
    {
    }

    Vector2(Vector2 const& other)
        : x(other.x)
        , y(other.y)
    {
    }

    Vector2(Vector2&& other)
        : x(std::move(other.x))
        , y(std::move(other.y))
    {
    }

    Vector2(T const& x, T const& y)
        : x(x)
        , y(y)
    {
    }

    ~Vector2()
    {
    }

    Vector2& operator=(Vector2 const& other)
    {
        if (&other != this)
        {
            x = other.x;
            y = other.y;
        }

        return *this;
    }

    Vector2& operator=(Vector2&& other)
    {
        if (&other != this)
        {
            x = std::move(other.x);
            y = std::move(other.y);
        }

        return *this;
    }

    Vector2& operator+=(Vector2 const& other)
    {
        x += other.x;
        y += other.y;

        return *this;
    }

    Vector2& operator+=(Vector2&& other)
    {
        x += std::move(other.x);
        y += std::move(other.y);

        return *this;
    }

    T x;
    T y;

    template<class T>
    friend Vector2<T> operator+(Vector2<T> const& lhs, Vector2<T> const& rhs);

    template<class T>
    friend Vector2<T> operator+(Vector2<T>&& lhs, Vector2<T>&& rhs);

    template<class T>
    friend bool operator==(Vector2<T> const& lhs, Vector2<T> const& rhs);
};

template<class T>
Vector2<T> operator+(Vector2<T> const& lhs, Vector2<T> const& rhs)
{
    return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<class T>
Vector2<T> operator+(Vector2<T>&& lhs, Vector2<T>&& rhs)
{
    return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<class T>
bool operator==(Vector2<T> const& lhs, Vector2<T> const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}