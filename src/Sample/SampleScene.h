#pragma once

#include "Scene.h"
#include <vector>

class DummyEntity;
class PhysicEntity;

class SampleScene : public Scene
{
    Rectangle m_rectUp;
    Rectangle m_rectBottom;
    Rectangle m_rectLeft;
    Rectangle m_rectRight;

    std::vector<PhysicEntity*> m_phEnt;

    PhysicEntity* m_selectedPHEnt;

    Segment m_viseur;
    float m_maxDistance = 400;

    float m_timerToHeld = 0.0f;

    float m_facteurPower = 1111;

public:
    void OnInitialize() override;
    void OnUpdate() override;
};
