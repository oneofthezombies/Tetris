#pragma once

#include <map>

#include "game_object.h"
#include "transform.h"

class GameObjectManager
{
public:
    static void update();
    static void render(HDC const& hdc);
    static void reset();
    static void join(GameObject* game_object);
    static void clear(tag_t const& tag);

    // TODO : 인스턴스 주소(GameObject*)를 받아서 주소, tag, id를 사용해서 빠르게 삭제하기
    /*
    
    e.g.) 
    
    call
    GameObjectManager::destroy(this);

    impl
    auto& group = _game_objects[go->tag()];
    for()
        if(go == *it)
            erase()
    */
    static void destroy(int const& id);
    static void destroy(GameObject* game_object);
    static std::deque<GameObject*> findGameObjectsWithTag(tag_t const& tag);
    static std::map<tag_t, std::deque<GameObject*>> gameObjects();
    static void actCollisions();
    static void registerCollision(tag_t const& actor, tag_t const& collosion);

    static void destroyLevels(float level);
    static void destroyPosition(Vector2<> position);

private:
    static int _game_object_id;
    static std::map<tag_t, std::deque<GameObject*>> _game_objects;
    static std::map<tag_t, std::vector<tag_t>> _collision_groups;
};
