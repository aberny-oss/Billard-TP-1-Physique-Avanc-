#pragma once
#include "Entity.h"

class PhysicEntity : public Entity
{
public:

    PhysicEntity();

    void OnInitialize() override;
    void OnUpdate() override;
    void OnCollision(Entity* other) override;

    void SetDecelerationSpeed(float p_decelerationSpeed) { m_decelerationSpeed = p_decelerationSpeed; }
    float GetDecelerationSpeed() { return m_decelerationSpeed; }

    void SetDestroy(bool p_bool) { m_destroyOK = p_bool; }
    bool IsDestroyOK() { return m_destroyOK; }

    void SetSpawnPoint(Vector2f p_spawn) { m_spawnPOs = p_spawn; }

private:

    float m_decelerationSpeed;
    bool m_destroyOK = true;
    Vector2f m_spawnPOs;

};

