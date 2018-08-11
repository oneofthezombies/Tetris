#pragma once

#include <Windows.h>
#include <memory>

#include "game_object_manager.h"
#include "random.h"
#include "input.h"
#include "time.h"

#include "block_factory.h"

class GameMain
{
public:
    GameMain()
        : _hbit(nullptr)
        , _level_timer(CoolTimer(0.01f, bind(&GameMain::levelEvent, this)))
        , _I_factor(0)
        , _score(0)
    {
    }

    virtual ~GameMain()
    {
    }

    virtual HRESULT init();
    virtual void reset();
    virtual void release();
    virtual void update();
    virtual void render(HDC const& hdc);
    virtual void renderProc(HWND hWnd, UINT message, UINT idEvent, DWORD time) final;

    void levelEvent();
    void fillLevelEventOrder();

    void CreateBitmap();
    void DrawBitmap(HWND hWnd, HDC hdc);

private:
    HBITMAP _hbit;
    deque<float> _level_events;
    deque<float> _level_event_order;
    CoolTimer _level_timer;
    int _I_factor;
    int _score;

    HBITMAP hNewImage;
    BITMAP bit;

    HBITMAP hNewImage2;
    BITMAP bit2;
};

struct GameMainDeleter
{
    void operator()(GameMain* self)
    {
        self->release();
        delete self;
    }
};

struct GameMainBuilder
{
    std::unique_ptr<GameMain, GameMainDeleter> operator()()
    {
        return std::unique_ptr<GameMain, GameMainDeleter>(new GameMain, GameMainDeleter());
    }
};

