#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"
#include "PhysicEntity.h"
#include <cmath>


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

    Vector2f topLeft = { 0, 0 };
    Vector2f topRight = { wWidth, 0 };
    Vector2f bottomLeft = { 0, wHeight };
    Vector2f bottomRight = { wWidth , wHeight };

    //create hole
    float holeRadius = 15;

    //up left
    PhysicEntity* upLeft = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    upLeft->SetPosition(topLeft,0, 0);
    upLeft->SetIsFixed(true);
    m_phEnt.push_back(upLeft);

    // up right
    PhysicEntity* upRight = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    upRight->SetPosition(topRight, 0, 1);
    upRight->SetIsFixed(true);
    m_phEnt.push_back(upRight);

    //down left
    PhysicEntity* downleft = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    downleft->SetPosition(bottomLeft, 0, 1);
    downleft->SetIsFixed(true);
    m_phEnt.push_back(downleft);

    //down right
    PhysicEntity* downRight = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    downRight->SetPosition(bottomRight, 1, 1);
    downRight->SetIsFixed(true);
    m_phEnt.push_back(downRight);

    //midle left
    PhysicEntity* midleLeft = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    midleLeft->SetPosition(bottomLeft * 0.5f);
    midleLeft->SetIsFixed(true);
    m_phEnt.push_back(midleLeft);

    //midle right
    PhysicEntity* midleRight = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    midleRight->SetPosition({ bottomRight.x, bottomRight.y * 0.5f });
    midleRight->SetIsFixed(true);
    m_phEnt.push_back(midleRight);


    float radius = 10.0f;
    float diameter = radius * 2.0f;
    float rowHeight = radius * std::sqrt(3.0f); 

    Vector2f placeFirst = { wWidth * 0.5f, wHeight * 0.2f };

    bool color = false;
    for (int i = 0; i < 5; ++i)
    {
        float startX = placeFirst.x - (i * radius);
        float posY = placeFirst.y - (i * rowHeight);

        for (int j = 0; j < i + 1; ++j)
        {
            float posX = startX + (j * diameter);

            PhysicEntity* phent = CreateEntity<PhysicEntity>(radius, Color::Red);
            phent->SetPosition({ posX, posY });

            if (color)
            {
                phent->SetColor(Color::Yellow);
                color = false;
            }
            else
            {
                color = true;
            }
            m_phEnt.push_back(phent);
        }
    }

    //White ball
    PhysicEntity* whiteBall = CreateEntity<PhysicEntity>(holeRadius, Color::White);
    whiteBall->SetPosition({wWidth * 0.5f, wHeight * 0.8f });
    whiteBall->SetDestroy(false);
    whiteBall->SetSpawnPoint(whiteBall->GetPosition());
    m_phEnt.push_back(whiteBall);

    m_viseur.a = { 0, 0 };
    m_viseur.b = { 0, 0 };

    m_selectedPHEnt = whiteBall;
}

void SampleScene::OnUpdate()
{
    float dt = GetDeltaTime();

    InputManager* im = InputManager::Get();
    const Vector2f mousePos = im->GetMousePosition();

    bool selectedent = false;
    if (im->IsMouseDown(SDL_BUTTON_RIGHT))
    {
        for (PhysicEntity* phent : m_phEnt)
        {
            if (phent->IsPointInside(mousePos))
            {
                if (m_selectedPHEnt)
                {
                    //m_selectedPHEnt->SetColor(Color::Red);
                }
                m_selectedPHEnt = phent;
                //m_selectedPHEnt->SetColor(Color::Cyan);
                selectedent = true;
                break;
            }
            else
            {
                if (m_selectedPHEnt)
                {
                    //m_selectedPHEnt->SetColor(Color::Red);
                    m_selectedPHEnt = nullptr;
                }
            }
        }
        if (selectedent)
        {
            return;
        }

        if (m_selectedPHEnt)
        {
            //Vector2f center = m_selectedPHEnt->GetPosition();

            //m_selectedPHEnt->SetVelocity(m_selectedPHEnt->GetVelocity() + (mousePos - center) * 0.69f);
        }
    }

    if (im->IsMouseHeld(SDL_BUTTON_LEFT))
    {
        if (m_selectedPHEnt)
        {
            m_timerToHeld += dt;

            Vector2f center = m_selectedPHEnt->GetPosition();

            Vector2f dir = mousePos - center;
            dir.Normalize();

            m_viseur.a = center;
            m_viseur.b = center - dir * m_timerToHeld * 100;
        }
    }
    if (im->IsMouseUp(SDL_BUTTON_LEFT))
    {
        if (m_selectedPHEnt)
        {
            Vector2f center = m_selectedPHEnt->GetPosition();

            Vector2f dir = mousePos - center;
            dir.Normalize();

            m_selectedPHEnt->SetVelocity(m_selectedPHEnt->GetVelocity() - dir * m_timerToHeld * 999);
            m_timerToHeld = 0.0;

            m_viseur.a = {0, 0};
            m_viseur.b = { 0, 0 };
        }
    }
    int WindowW = GetWindowWidth();
    int WindowH = GetWindowHeight();

    Vector2f topLeft = { 0, 0 };
    Vector2f topRight = { WindowW, 0 };
    Vector2f bottomLeft = { 0, WindowH };
    Vector2f bottomRight = { WindowW , WindowH };

    int margin = 3;
    //up
    Debug::DrawLine(topLeft + margin, topRight + margin, Color::Green);
    //down
    Debug::DrawLine(bottomLeft - margin, bottomRight - margin, Color::Green);
    //left
    Debug::DrawLine(topLeft + margin, bottomLeft + margin, Color::Green);
    //right
    Debug::DrawLine(topRight - margin, bottomRight - margin, Color::Green);

    //viseur
    Vector2f Viseur = m_viseur.b - m_viseur.a;
    float lenghtViseur = Viseur.Magnitude();
    if (lenghtViseur <= 150)
    {
        Debug::DrawLine(m_viseur.a, m_viseur.b, Color::Yellow);
    }
    else if (lenghtViseur <= 250)
    {
        Debug::DrawLine(m_viseur.a, m_viseur.b, Color::Orange);
    }
    else
    {
        Debug::DrawLine(m_viseur.a, m_viseur.b, Color::Red);
    }




}
