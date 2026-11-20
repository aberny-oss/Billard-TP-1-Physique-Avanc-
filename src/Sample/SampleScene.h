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
    bool m_upOrDown = false;

    float m_timerToHeld = 0.0f;
    float m_maxTimeToHeld = 1.8f;

    float m_facteurPower = 666;

    const float m_ratioMP = 100.0f; // 1m = 100pixels
    Vector2f m_gravity = { 0.f, 9.81f * m_ratioMP };

    Circle m_pointcollideTest;
    Segment m_segmentCollideTest;

public:
    void OnInitialize() override;
    void OnUpdate() override;
};
