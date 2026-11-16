#include "Entity.h"

#include "GameManager.h"
#include "Debug.h"

#include <cassert>
#include <SDL3/SDL_render.h>

void Entity::Initialize(float radius, const Color& color)
{
    assert(radius > 0.f);

    mCircle.position = { 0.f, 0.f };
    mCircle.radius = radius;
    mCircle.color = color;
    mCircle.filled = true;

    OnInitialize();
}

void Entity::Destroy()
{
    if (mToDestroy)
        return;

    mToDestroy = true;

    OnDestroy();
}

void Entity::SetPosition(Vector2f position, float ratioX, float ratioY)
{
    float size = mCircle.radius * 2.f;

    position.x -= size * ratioX;
    position.y -= size * ratioY;

    mCircle.position = position;
}

Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
    float size = mCircle.radius * 2.f;
    Vector2f position = mCircle.position;

    position.x += size * ratioX;
    position.y += size * ratioY;

    return position;
}

void Entity::Update()
{
    float dt = GetDeltaTime();

    mCircle.position += m_velocity * dt;

    //Vector2f bottomRight = GetPosition(1.0f, 1.0f);
    //Vector2f upLeft = GetPosition(0.0f, 0.0f);

    //Scene* sc = GetScene();
    //int WWidth = sc->GetWindowWidth();
    //int WHeight = sc->GetWindowHeight();

    ////Line Up
    //Line up;
    //up.normal = { 0, -1 };

    ////Line Down
    //Line down;
    //down.normal = { 0, 1 };

    ////Line Left
    //Line left;
    //left.normal = { -1, 0 };

    ////Line Right
    //Line right;
    //right.normal = { 1, 0 };

    ////Bord Right
    //float overlapRight = bottomRight.x - WWidth;
    //if (overlapRight > 0)
    //{
    //    mCircle.position.x -= overlapRight;
    //    m_velocity.x *= -1;
    //    m_velocity.x *= 0.5f;
    //}

    ////Bord Down
    //float overlapDown = bottomRight.y - WHeight;
    //if (overlapDown > 0)
    //{
    //    mCircle.position.y -= overlapDown;
    //    m_velocity.y *= -1;
    //    m_velocity.y *= 0.5f;
    //}

    ////Bord Left
    //float overlapLeft = upLeft.x;
    //if (overlapLeft < 0)
    //{
    //    mCircle.position.x -= overlapLeft;
    //    m_velocity.x *= -1;
    //    m_velocity.x *= 0.5f;
    //}

    ////Bord Up
    //float overlapUp = upLeft.y;
    //if (overlapUp < 0)
    //{
    //    mCircle.position.y -= overlapUp;
    //    m_velocity.y *= -1;
    //    m_velocity.y *= 0.5f;
    //}

    OnUpdate();
}

void Entity::Draw(SDL_Renderer* renderer) const
{
    mCircle.Draw(renderer);
}

Scene* Entity::GetScene() const
{
    return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
    return GameManager::Get()->GetDeltaTime();
}

void Entity::SetVelocity(Vector2f p_velocity)
{
    m_velocity = p_velocity;
}

bool Entity::IsPointInside(Vector2f p_point)
{
    Vector2f topLeft = GetPosition(0, 0);
    Vector2f bottomRight = GetPosition(1, 1);

    if (p_point.x >= topLeft.x && p_point.y >= topLeft.y && p_point.x <= bottomRight.x && p_point.y <= bottomRight.y)
    {
        return true;
    }
    return false;
}
