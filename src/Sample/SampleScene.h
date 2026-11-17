#pragma once

#include "Scene.h"
#include <vector>

class DummyEntity;
class PhysicEntity;

class SampleScene : public Scene
{

    Line m_lineUp;
    Line m_lineDown;
    Line m_lineLeft;
    Line m_lineRight;

    std::vector<PhysicEntity*> m_phEnt;

    PhysicEntity* m_selectedPHEnt;

    Segment m_viseur;

    float m_timerToHeld = 0.0f;

public:
    void OnInitialize() override;
    void OnUpdate() override;
};
