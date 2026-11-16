#pragma once
#include "Entity.h"

class PhysicEntity : public Entity
{
public:

    PhysicEntity();

    void OnInitialize() override;
    void OnUpdate() override;
    void OnCollision(Entity* other) override;

private:




};

