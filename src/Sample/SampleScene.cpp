#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"
#include "PhysicEntity.h"


void SampleScene::OnInitialize()
{
    GameManager* gm = GameManager::Get();

    int wWidth = GetWindowWidth();
    int wHeight = GetWindowHeight();

    m_lineUp;
    m_lineUp.normal = { 0, -1 };
    m_lineUp.point = { 0, 0 };
    gm->CreateLine(m_lineUp);

    m_lineDown;
    m_lineDown.normal = { 0, 1 };
    m_lineDown.point = { wWidth, wHeight };
    gm->CreateLine(m_lineDown);

    m_lineLeft;
    m_lineLeft.normal = { -1, 0 };
    m_lineLeft.point = { 0, 0 };
    gm->CreateLine(m_lineLeft);

    m_lineRight;
    m_lineRight.normal = { 1, 0 };
    m_lineRight.point = { wWidth, wHeight };
    gm->CreateLine(m_lineRight);

    m_circleFixed = CreateEntity<Entity>(25.0f, Color::Magenta);
    m_circleFixed->SetPosition({ wWidth * 0.5f, wHeight * 0.5f });

    for (int i = 0; i < 3; ++i)
    {
        PhysicEntity* phent = CreateEntity<PhysicEntity>(10.f, Color::Red);
        phent->SetPosition({ 100 + i * 50, 50 });
        m_phEnt.push_back(phent);
    }

    m_rectangle.position = { 200, 150 };
    m_rectangle.size = { 150, 150 };
    m_rectangle.color = Color::Green;
    m_rectangle.filled = false;
    gm->CreateRectangle(m_rectangle);



}

void SampleScene::OnUpdate()
{
    InputManager* im = InputManager::Get();

    if (im->IsMouseDown(SDL_BUTTON_LEFT))
    {
        bool selectedent = false;
        Vector2f mousePos = im->GetMousePosition();
        for (PhysicEntity* phent : m_phEnt)
        {
            if (phent->IsPointInside(mousePos))
            {
                if (m_selectedPHEnt)
                {
                    m_selectedPHEnt->SetColor(Color::Red);
                }
                m_selectedPHEnt = phent;
                m_selectedPHEnt->SetColor(Color::Cyan);
                selectedent = true;
                break;
            }
        }
        if (selectedent)
        {
            return;
        }

        if (m_selectedPHEnt)
        {
            Vector2f center = m_selectedPHEnt->GetPosition();

            m_selectedPHEnt->SetVelocity(m_selectedPHEnt->GetVelocity() + (mousePos - center) * 0.69f);
        }
    }
    if (im->IsMouseDown(SDL_BUTTON_RIGHT))
    {
        if (m_selectedPHEnt)
        {
            m_selectedPHEnt->SetColor(Color::Red);
            m_selectedPHEnt = nullptr;
        }
    }

    Debug::DrawRectangle(m_rectangle.position, m_rectangle.size, m_rectangle.color, m_rectangle.filled);

    
}
