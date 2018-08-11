#include "stdafx.h"
#include "game_object_manager.h"

int GameObjectManager::_game_object_id = 0;
std::map<tag_t, std::deque<GameObject*>> GameObjectManager::_game_objects;
std::map<tag_t, std::vector<tag_t>> GameObjectManager::_collision_groups;

void GameObjectManager::update()
{
    for (auto group : _game_objects)
        for (auto& go : group.second)
            go->updateRec();
}

void GameObjectManager::render(HDC const& hdc)
{
    for (auto& group : _game_objects)
        for (auto& go : group.second)
            go->renderRec(hdc);
}

void GameObjectManager::reset()
{
    for (auto& group : _game_objects)
        for (auto& go : group.second)
            go->releaseRec();

    _game_objects.clear();
    _game_object_id = 0;
}

void GameObjectManager::join(GameObject* game_object)
{
    game_object->instanceId(_game_object_id);
    _game_object_id++;

    _game_objects[game_object->tag()].emplace_back(game_object);
}

/*

TODO : 릴리즈 딜리트 정리하기

*/
void GameObjectManager::clear(tag_t const& tag)
{
    _game_objects[tag].clear();
}

void GameObjectManager::destroy(int const& id)
{
    for (auto& group : _game_objects)
        for (auto it = group.second.begin(); it != group.second.end();)
            if ((*it)->instanceId() == id)
                it = group.second.erase(it);
            else
                ++it;
}

void GameObjectManager::destroy(GameObject* game_object)
{
    for (auto& group : _game_objects)
        for (auto it = group.second.begin(); it != group.second.end();)
            if ((*it) == game_object)
                it = group.second.erase(it);
            else
                ++it;
}

std::deque<GameObject*> GameObjectManager::findGameObjectsWithTag(tag_t const& tag)
{
    return _game_objects[tag];
}

std::map<tag_t, std::deque<GameObject*>> GameObjectManager::gameObjects()
{
    return _game_objects;
}

void GameObjectManager::actCollisions()
{
    for (auto& tag_group : _collision_groups)
        for (auto& actor : _game_objects[tag_group.first])
            for (auto& tag : tag_group.second)
                for (auto& go : _game_objects[tag])
                {
                    RECT area;
                    if (IntersectRect(&area,
                                      &actor->getComponent<Transform>()->toRect(),
                                      &go->getComponent<Transform>()->toRect()))
                    {
                        go->onCollisionEnter(actor, area);
                    }
                }
}

void GameObjectManager::registerCollision(tag_t const& actor, tag_t const& collosion)
{
    _collision_groups[actor].emplace_back(collosion);
}

void GameObjectManager::destroyLevels(float level)
{
    for (auto& group : _game_objects)
    {
        for (auto& it = group.second.begin(); it != group.second.end();)
        {
            auto tr = (*it)->getComponent<Transform>();
            if (tr->position().y == level)
                it = group.second.erase(it);
            else
                ++it;
        }
    }
}

void GameObjectManager::destroyPosition(Vector2<> position)
{
    for (auto& group : _game_objects)
    {
        for (auto& it = group.second.begin(); it != group.second.end();)
        {
            auto tr = (*it)->getComponent<Transform>();
            if (tr->position().y == position.y && tr->position().x == position.x)
                it = group.second.erase(it);
            else
                ++it;
        }
    }
}
