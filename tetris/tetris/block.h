#pragma once

#include "game_object.h"
#include "cool_timer.h"
#include "transform.h"
#include "input.h"

class Block : public GameObject
{
public:
    Block(tag_t const& tag, Vector2<> const& position)
        : GameObject()
        , _to_ground(1.0f, []() {})
    {
        this->tag(tag);
        setComponent<Transform>();
        _transform = getComponent<Transform>();
        _transform->position(position);
    }

    virtual void update() override
    {
        if (_tag == tag_t::kCurrent)
        {
            Vector2<> _future_position = _transform->position();
            std::vector<Vector2<>> ch_pos;
            for (auto c : _children)
                ch_pos.emplace_back(c->getComponent<Transform>()->globalPosition());
            
            if (Input::isOnceKeyDown(VK_LEFT))
            {
                _future_position.x -= 1.0f;
                for (auto& p : ch_pos)
                    p.x -= 1.0f;
            }

            if (Input::isOnceKeyDown(VK_RIGHT))
            {
                _future_position.x += 1.0f;
                for (auto& p : ch_pos)
                    p.x += 1.0f;
            }

            if (Input::isStayKeyDown(VK_DOWN))
            {
                _future_position.y += 1.0f;
                for (auto& p : ch_pos)
                    p.y += 1.0f;
            }

            bool is_pressed_up = false;
            if (Input::isOnceKeyDown(VK_UP))
            {
                is_pressed_up = true;

                for (unsigned i = 0; i < _children.size(); ++i)
                {
                    auto tr = _children[i]->getComponent<Transform>();
                    ch_pos[i] = rotate(tr->position()) + _transform->position();
                }
            }

            if (_to_ground())
            {
                _future_position.y += 1.0f;
                for (auto& p : ch_pos)
                    p.y += 1.0f;
            }

            bool is_out_of_range = false;
            bool hit_bottom = false;
            for (auto p : ch_pos)
            {
                if (p.x < 1.0f || p.x > 10.0f)
                    is_out_of_range = true;

                if (p.y == 20.0f)
                    hit_bottom = true;
            }

            bool is_collided = false;
            for (auto s : GameObjectManager::findGameObjectsWithTag(tag_t::kStacked))
            {
                auto s_pos = s->getComponent<Transform>()->position();
                for (auto p : ch_pos)
                {
                    if (p == s_pos)
                        is_collided = true;

                    if (p.y + 1.0f == s_pos.y && p.x == s_pos.x)
                        hit_bottom = true;
                }
            }

            if (!is_collided && !is_out_of_range)
            {
                _transform->position(_future_position);

                if (is_pressed_up)
                {
                    for (auto c : _children)
                    {
                        auto tr = c->getComponent<Transform>();
                        tr->position(rotate(tr->position()));
                    }
                }

                if (hit_bottom)
                {
                    for (auto c : _children)
                    {
                        c->tag(tag_t::kStacked);
                        auto tr = c->getComponent<Transform>();
                        tr->position(tr->globalPosition());
                        c->parent(nullptr);
                        GameObjectManager::join(c);
                    }

                    _children.resize(0);
                    GameObjectManager::clear(tag_t::kCurrent);
                }
            }
        }
    }

    Vector2<> rotate(Vector2<> const& position)
    {
        return Vector2<>(-position.y, position.x);
    }

    virtual void render(HDC const& hdc)
    {
    }

protected:
    Transform* _transform;

private:
    CoolTimer _to_ground;
};

class Piece : public Block
{
public:
    Piece(tag_t const& tag, Vector2<> const& position, COLORREF const& color)
        : Block(tag, position)
        , _color(color)
    {
        _transform->size(30, 30);
    }

    virtual void update() override
    {
    }

    virtual void render(HDC const& hdc) override
    {
        Pen(hdc, 2, _color)([&hdc, this]() 
        {
            auto pos = _transform->globalPosition();
            auto size = _transform->size();
            DrawRectangle(hdc, pos.x * size.x - 10.0f, pos.y * size.y - 10.0f, size.x, size.y);
        });
    }

private:
    COLORREF _color;
};
