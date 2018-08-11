#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <type_traits>
#include <functional>
#include <Windows.h>

#include "vector2.h"

struct Color
{
    static const COLORREF kBlack = RGB(0, 0, 0);
    static const COLORREF kGray = RGB(127, 127, 127);
    static const COLORREF kWhite = RGB(255, 255, 255);
    static const COLORREF kRed = RGB(255, 0, 0);
    static const COLORREF kGreen = RGB(0, 255, 0);
    static const COLORREF kBlue = RGB(0, 0, 255);
    static const COLORREF kCyan = RGB(0, 255, 255);
    static const COLORREF kMagenta = RGB(255, 0, 255);
    static const COLORREF kYellow = RGB(255, 255, 0);
};

struct Font
{
    Font(HDC const& hdc, int const& size = 1)
        : hdc(hdc)
        , size(size)
    {
    }

    void operator()(std::function<void()> const& functor)
    {
        HFONT font = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("Malgun"));
        HFONT oldfont = (HFONT)SelectObject(hdc, font);
        
        functor();

        SelectObject(hdc, oldfont);
        DeleteObject(font);
    }

    HDC hdc;
    int size;
};

struct Pen
{
    Pen(HDC const& hdc, int const& width = 1, COLORREF const& color = Color::kBlack)
        : hdc(hdc)
        , width(width)
        , color(color)
    {
    }

    void operator()(std::function<void()> const& functor)
    {
        HBRUSH brush = CreateSolidBrush(color);
        HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN pen = CreatePen(PS_SOLID, width, Color::kWhite);
        HPEN oldpen = (HPEN)SelectObject(hdc, pen);
        
        functor();

        SelectObject(hdc, oldbrush);
        SelectObject(hdc, oldpen);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    HDC hdc;
    int width;
    COLORREF color;
};

template 
    < typename T1
    , typename T2
    , typename T3
    , typename T4
    >
RECT makeRect(T1&& x, T2&& y, T3&& width, T4&& height)
{
    return RECT
    {
        static_cast<LONG>(x), static_cast<LONG>(y),
        static_cast<LONG>(x + static_cast<T1>(std::forward<T3>(width))), static_cast<LONG>(y + static_cast<T2>(std::forward<T4>(height)))
    };
}

template 
    < typename T1
    , typename T2
    , typename T3
    , typename T4
    >
RECT makeRectCenter(T1&& x, T2&& y, T3&& width, T4&& height)
{
    T1 half_width = static_cast<T1>(width / 2);
    T2 half_height = static_cast<T2>(height / 2);

    return RECT
    {
        static_cast<LONG>(x - half_width), static_cast<LONG>(y - half_height),
        static_cast<LONG>(x + half_width), static_cast<LONG>(y + half_height)
    };
}

template 
    < typename T1
    , typename T2
    >
RECT makeRectCenter(Vector2<T1> const& position, Vector2<T2> const& size)
{
    T1 half_width = size.x / 2;
    T2 half_height = size.y / 2;

    return RECT
    {
        static_cast<LONG>(position.x - half_width), static_cast<LONG>(position.y - half_height),
        static_cast<LONG>(position.x + half_width), static_cast<LONG>(position.y + half_height)
    };
}

template
    < typename T1
    , typename T2
    , typename T3
    , typename T4
    >
void DrawLine(HDC const& hdc, T1&& source_x, T2&& source_y, T3&& dest_x, T4&& dest_y)
{
    MoveToEx(hdc, static_cast<int>(std::forward<T1>(source_x)), static_cast<int>(std::forward<T2>(source_y)), NULL);
    LineTo(hdc, static_cast<int>(std::forward<T3>(dest_x)), static_cast<int>(std::forward<T4>(dest_y)));
}

template
    < typename T1
    , typename T2
    , typename T3
    , typename T4
    >
void DrawRectangle(HDC const& hdc, T1&& center_x, T2&& center_y, T3&& width, T4&& height)
{
    T1 half_width = static_cast<T1>(std::forward<T3>(width) / 2);
    T2 half_height = static_cast<T2>(std::forward<T4>(height) / 2);

    Rectangle(hdc,
              static_cast<int>(center_x - half_width), static_cast<int>(center_y - half_height),
              static_cast<int>(center_x + half_width), static_cast<int>(center_y + half_height));
}

template
    < typename T1
    , typename T2
    , typename T3
    >
void DrawCircle(HDC const& hdc, T1&& center_x, T2&& center_y, T3&& radius)
{
    Ellipse(hdc,
            static_cast<int>(center_x - radius), static_cast<int>(center_y - radius),
            static_cast<int>(center_x + radius), static_cast<int>(center_y + radius));
}

template
    < typename T1
    , typename T2
    , typename T3
    >
void DrawStar(HDC const& hdc, T1&& center_x, T2&& center_y, T3&& radius)
{
    POINT points[5];
    for (unsigned i = 0; i < 5; ++i)
    {
        points[i].x = static_cast<LONG>(static_cast<double>(std::forward<T1>(center_x)) + sin(2.0 * M_PI * static_cast<double>((i * 2 % 5)) / 5.0) * static_cast<double>(radius) / 2.0);
        points[i].y = static_cast<LONG>(static_cast<double>(std::forward<T2>(center_y)) - cos(2.0 * M_PI * static_cast<double>((i * 2 % 5)) / 5.0) * static_cast<double>(radius) / 2.0);
    }

    SetPolyFillMode(hdc, WINDING);
    Polygon(hdc, points, 5);
}

/*

요지는 인자로 함수포인터 또는 STL 함수 오브젝트를 넘기는 것이다.
#include <functional>

*/

//.h
void textRender(std::function<void()> functor);

//.cpp
void textRender(std::function<void()> functor)
{

}

// vs

//.h
struct textRenderer
{
    textRenderer(HDC hdc, COLORREF color)
    {}

    void operator()(std::function<void()> functor)
    {

    }

    operator bool()
    {

    }

    HDC hdc;
    COLORREF color;
};
