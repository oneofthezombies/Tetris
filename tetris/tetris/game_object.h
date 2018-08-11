#pragma once

#include <unordered_map>
#include <Windows.h>
#include <vector>
#include <deque>

#include "component.h"
#include "layer.h"
#include "tag.h"

class GameObject
{
public:
    GameObject()
        : _instance_id(-1)
        , _tag(tag_t::kIdle)
        , _layer(layer_t::kIdle)
        , _components(std::unordered_map<size_t, Component*>{})
        , _parent(nullptr)
        , _children(std::deque<GameObject*>{})
    {
    }

    virtual ~GameObject()
    {
        for (auto c : _children)
            delete c;

        for (auto pair : _components)
            delete pair.second;
    }

    virtual void init() 
    {
    }

    virtual void release() 
    {
    }

    virtual void update()
    {
    }

    virtual void render(HDC const& hdc)
    {
    }

    virtual void onCollisionEnter(GameObject* other, RECT const& area)
    {
    }

    virtual void initRec() final
    {
        init();

        for (auto c : _children)
            c->initRec();
    }

    virtual void releaseRec() final
    {
        for (auto c : _children)
            c->releaseRec();

        release();
    }

    virtual void updateRec() final    
    {
        update();

        for (auto c : _children)
            c->updateRec();
    }

    virtual void renderRec(HDC const& hdc) final
    {
        render(hdc);

        for (auto c : _children)
            c->renderRec(hdc);
    }

    virtual void parent(GameObject* parent) final
    {
        _parent = parent;
    }

    virtual GameObject* parent() const final
    {
        return _parent;
    }

    virtual void setChild(GameObject* child) final
    {
        child->parent(this);
        _children.emplace_back(child);
    }

    virtual std::deque<GameObject*> children() const final
    {
        return _children;
    }

    virtual void instanceId(int const& instanceId) final
    {
        _instance_id = instanceId;
    }

    virtual int instanceId() const final
    {
        return _instance_id;
    }

    virtual void tag(tag_t const& _tag) final 
    {
        this->_tag = _tag;
    }

    virtual tag_t tag() const final 
    {
        return _tag;
    }

    virtual void layer(layer_t const& _layer) final
    {
        this->_layer = _layer;
    }

    virtual layer_t layer() const final
    {
        return _layer;
    }

    template<class T>
    void setComponent()
    {
#ifdef OOTZ_DEBUG
        assert((STD is_base_of<Component, T>::value));
#endif
        size_t key = typeid(T).hash_code();
        if (_components.find(key) == _components.end())
            _components.insert(std::make_pair(key, new T(this)));
    }

    template<class T>
    T* getComponent() const
    {
        auto pair = _components.find(typeid(T).hash_code());
        if (pair != _components.end())
            return static_cast<T*>(pair->second);

        return nullptr;
    }

protected:
    int _instance_id;
    tag_t _tag;
    layer_t _layer;
    std::unordered_map<size_t, Component*> _components;

    GameObject* _parent;
    std::deque<GameObject*> _children;
};
