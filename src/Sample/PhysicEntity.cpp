#include "PhysicEntity.h"
#include <LightEngine/InputManager.h>
#include "SampleScene.h"
#include <algorithm>

PhysicEntity::PhysicEntity()
{
    m_decelerationSpeed = 164;
}

void PhysicEntity::OnInitialize()
{
}

void PhysicEntity::OnUpdate()
{
    float dt = GetDeltaTime();
    float decel = m_decelerationSpeed * dt;

    float speed = m_velocity.Magnitude();
    float newSpeed = speed - decel;

    if(speed > 0)
        m_velocity = m_velocity * (newSpeed / speed);
    //m_velocity.x = std::max(m_velocity.x , 0.0f);
    //m_velocity.y = std::max(m_velocity.y, 0.0f);

    int a = 0;

}

void PhysicEntity::OnCollision(Entity* other)
{
    if (other->IsFixed())
    {
        if (m_destroyOK)
        {
            Destroy();
        }
        else
        {
            SetPosition(m_spawnPOs);
            m_velocity = { 0, 0 };
        }
    }
}
