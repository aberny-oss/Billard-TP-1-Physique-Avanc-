#pragma once

#include "Scene.h"
#include <vector>

class DummyEntity;
class PhysicEntity;

class SampleScene : public Scene
{
    Rectangle m_rectangle;

    Line m_lineUp;
    Line m_lineDown;
    Line m_lineLeft;
    Line m_lineRight;

    std::vector<PhysicEntity*> m_phEnt;

    PhysicEntity* m_selectedPHEnt;

    Entity* m_circleFixed;

public:
    void OnInitialize() override;
    void OnUpdate() override;
};
