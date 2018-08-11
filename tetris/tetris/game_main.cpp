#include "stdafx.h"
#include "game_main.h"

HRESULT GameMain::init()
{
    GameObjectManager::join(BlockFactory {}(static_cast<BlockFactory::Shape>(Randomer {}()->getIntFromTo(1, 7))));
    GameObject* current = BlockFactory {}(static_cast<BlockFactory::Shape>(Randomer {}()->getIntFromTo(1, 7)), tag_t::kCurrent);
    current->getComponent<Transform>()->position(5.0f, 2.0f);
    GameObjectManager::join(current);

    //CreateBitmap();

    return S_OK;
}

void GameMain::reset()
{
    Time::reset();
    Input::reset();
}

void GameMain::release()
{
    if (_hbit)
        DeleteObject(_hbit);
}

void GameMain::update()
{
    Time::update();

    if (GameObjectManager::findGameObjectsWithTag(tag_t::kCurrent).empty())
    {
        GameObject* next = GameObjectManager::findGameObjectsWithTag(tag_t::kNext).front();
        next->tag(tag_t::kCurrent);
        next->getComponent<Transform>()->position(5.0f, 2.0f);
        GameObjectManager::join(next);
        GameObjectManager::clear(tag_t::kNext);
        int num = Randomer {}()->getIntFromTo(1, 7);
        if (num != 6) _I_factor++;
        if (_I_factor == 10)
        {
            num = 6;
            _I_factor = 0;
        }
        GameObjectManager::join(BlockFactory {}(static_cast<BlockFactory::Shape>(num)));
    }

    GameObjectManager::update();
    
    if (_level_events.empty())
    {
        map<float, vector<GameObject*>> m;
        for (auto go : GameObjectManager::findGameObjectsWithTag(tag_t::kStacked))
            m[go->getComponent<Transform>()->position().y].emplace_back(go);

        for (auto it = m.rbegin(); it != m.rend(); ++it)
            if (it->second.size() == 10u)
            {
                float level = it->second.front()->getComponent<Transform>()->position().y;
                _level_events.emplace_back(level);
                fillLevelEventOrder();
            }

        _score += _level_events.size() * _level_events.size() * 100;

        if (!m[2.0f].empty() || !m[1.0f].empty() || !m[3.0f].empty())
            SendMessage(hWnd, WM_DESTROY, 0, 0);
    }
    else
        _level_timer();
}

void GameMain::render(HDC const& hdc)
{
    Pen(hdc, 1)([&hdc]() 
    {
        DrawRectangle(hdc, 317, 400, 20, 800);
    });

    GameObjectManager::render(hdc);

    Font(hdc, 20)([&hdc, this]()
    {
        wstring str = TEXT("점수");
        TextOut(hdc, 350, 500, str.c_str(), str.size());
        str = to_wstring(_score);
        TextOut(hdc, 350, 550, str.c_str(), str.size());
    });

    //DrawBitmap(hWnd, hdc);
}

void GameMain::renderProc(HWND hWnd, UINT message, UINT idEvent, DWORD time)
{
    update();

    HDC hdc = GetDC(hWnd);
    RECT cr;
    GetClientRect(hWnd, &cr);
    if (_hbit == nullptr)
        _hbit = CreateCompatibleBitmap(hdc, cr.right, cr.bottom);

    HDC hmemdc = CreateCompatibleDC(hdc);
    HBITMAP oldbit = (HBITMAP)SelectObject(hmemdc, _hbit);
    PatBlt(hmemdc, 0, 0, cr.right, cr.bottom, WHITENESS);
    SetBkMode(hmemdc, TRANSPARENT);

    render(hmemdc);

    BitBlt(hdc, 0, 0, cr.right, cr.bottom, hmemdc, 0, 0, SRCCOPY);
    SelectObject(hmemdc, oldbit);
    DeleteDC(hmemdc);
    ReleaseDC(hWnd, hdc);
}

void GameMain::levelEvent()
{
    float x = _level_event_order.front();
    _level_event_order.pop_front();

    float y = _level_events.front();
    GameObjectManager::destroyPosition(Vector2<>(x, y));

    if (_level_event_order.empty())
    {
        _level_events.pop_front();
        for (auto& level : _level_events)
            level++;

        for (auto& group : GameObjectManager::gameObjects())
            for (auto& go : group.second)
            {
                auto tr = go->getComponent<Transform>();
                if (tr->position().y < y)
                    tr->moveY(1);
            }

        if (!_level_events.empty())
            fillLevelEventOrder();
    }
}

void GameMain::fillLevelEventOrder()
{
    _level_event_order.resize(0);
    _level_event_order.emplace_back(5.0f);
    _level_event_order.emplace_back(6.0f);
    _level_event_order.emplace_back(4.0f);
    _level_event_order.emplace_back(7.0f);
    _level_event_order.emplace_back(3.0f);
    _level_event_order.emplace_back(8.0f);
    _level_event_order.emplace_back(2.0f);
    _level_event_order.emplace_back(9.0f);
    _level_event_order.emplace_back(1.0f);
    _level_event_order.emplace_back(10.0f);
}

void GameMain::CreateBitmap()
{
    hNewImage = (HBITMAP)LoadImage(NULL, TEXT("Images/수지.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    GetObject(hNewImage, sizeof(BITMAP), &bit);

    hNewImage2 = (HBITMAP)LoadImage(NULL, TEXT("Images/sigong.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    GetObject(hNewImage2, sizeof(BITMAP), &bit2);
}

void GameMain::DrawBitmap(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    hMemDC = CreateCompatibleDC(hdc);
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hNewImage);
    bx = bit.bmWidth;
    by = bit.bmHeight;
    BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

    hMemDC = CreateCompatibleDC(hdc);
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hNewImage2);
    bx = bit2.bmWidth;
    by = bit2.bmHeight;
    TransparentBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}
