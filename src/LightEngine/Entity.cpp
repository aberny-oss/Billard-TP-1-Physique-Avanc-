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

    m_velocity = { 0, 0 };
    m_acceleration = { 0, 0 };

    OnInitialize();
}

void Entity::FixeUpdate(float dt)
{
    m_velocity += m_mass * m_acceleration * dt;
    //if (m_velocity.Magnitude() < 3.0f)
    //{
    //    m_velocity = { 0, 0 };
    //}
    mCircle.position += m_velocity * dt;
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
