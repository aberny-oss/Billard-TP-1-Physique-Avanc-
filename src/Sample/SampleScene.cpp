#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"
#include "PhysicEntity.h"
#include <cmath>
#include <algorithm>
#include <stdlib.h>

void SampleScene::OnInitialize()
{
    GameManager* gm = GameManager::Get();

    int wWidth = GetWindowWidth();
    int wHeight = GetWindowHeight();

    Vector2f topLeft = { 0, 0 };
    Vector2f topRight = { wWidth, 0 };
    Vector2f bottomLeft = { 0, wHeight };
    Vector2f bottomRight = { wWidth , wHeight };

    //create hole
    float holeRadius = 15;
    float margin = 6;

    //up left
    PhysicEntity* upLeft = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    upLeft->SetPosition(topLeft + margin,0, 0);
    upLeft->SetIsFixed(true);
    m_phEnt.push_back(upLeft);

    // up right
    PhysicEntity* upRight = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    upRight->SetPosition({ topRight.x - margin, topRight.y + margin }, 1, 0);
    upRight->SetIsFixed(true);
    m_phEnt.push_back(upRight);

    //down left
    PhysicEntity* downleft = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    downleft->SetPosition({ bottomLeft.x + margin, bottomLeft.y - margin }, 0, 1);
    downleft->SetIsFixed(true);
    m_phEnt.push_back(downleft);

    //down right
    PhysicEntity* downRight = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    downRight->SetPosition(bottomRight - margin, 1, 1);
    downRight->SetIsFixed(true);
    m_phEnt.push_back(downRight);

    //midle left
    PhysicEntity* midleLeft = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    midleLeft->SetPosition({ bottomLeft.x * 0.5f + margin, bottomLeft.y * 0.5f }, 0, 0.5f);
    midleLeft->SetIsFixed(true);
    m_phEnt.push_back(midleLeft);

    //midle right
    PhysicEntity* midleRight = CreateEntity<PhysicEntity>(holeRadius, Color::Black);
    midleRight->SetPosition({ bottomRight.x - margin, bottomRight.y * 0.5f }, 1, 0.5f);
    midleRight->SetIsFixed(true);
    m_phEnt.push_back(midleRight);

    //rect up
    m_rectUp.position = topLeft;
    m_rectUp.size = { (float)wWidth, holeRadius };
    m_rectUp.color = Color::Green;
    m_rectUp.filled = true;
    gm->CreateRectangle(m_rectUp);

    //rect down
    m_rectBottom.position = { bottomLeft.x, bottomLeft.y - holeRadius };
    m_rectBottom.size = { (float)wWidth, holeRadius };
    m_rectBottom.color = Color::Green;
    m_rectBottom.filled = true;
    gm->CreateRectangle(m_rectBottom);

    //rect left
    m_rectLeft.position = topLeft;
    m_rectLeft.size = { holeRadius, (float)wHeight };
    m_rectLeft.color = Color::Green;
    m_rectLeft.filled = true;
    gm->CreateRectangle(m_rectLeft);

    //rect right
    m_rectRight.position = { topRight.x - holeRadius, topRight.y };
    m_rectRight.size = { holeRadius, (float)wHeight };
    m_rectRight.color = Color::Green;
    m_rectRight.filled = true;
    gm->CreateRectangle(m_rectRight);


    float radius = 10.0f;
    float diameter = radius * 2.0f;
    float rowHeight = radius * std::sqrt(3.0f); 

    Vector2f placeFirst = { wWidth * 0.5f, wHeight * 0.4f };

    bool color = false;
    for (int i = 0; i < 15; ++i)
    {
        float startX = placeFirst.x - (i * radius);
        float posY = placeFirst.y - (i * rowHeight);

        for (int j = 0; j < i + 1; ++j)
        {
            float posX = startX + (j * diameter);

            Color colorRand = { (uint8)(std::rand() % 256), (uint8)(std::rand() % 256), (uint8)(std::rand() % 256) };

            PhysicEntity* phent = CreateEntity<PhysicEntity>(radius, colorRand);
            phent->SetPosition({ posX, posY });
            phent->SetAcceleration(m_gravity);

            //if (color)
            //{
            //    phent->SetColor(Color::Yellow);
            //    color = false;
            //}
            //else
            //{
            //    color = true;
            //}
            m_phEnt.push_back(phent);
        }
    }

    //White ball
    PhysicEntity* whiteBall = CreateEntity<PhysicEntity>(holeRadius, Color::White);
    whiteBall->SetPosition({wWidth * 0.5f, wHeight * 0.8f });
    whiteBall->SetDestroy(false);
    whiteBall->SetSpawnPoint(whiteBall->GetPosition());
    whiteBall->SetAcceleration(m_gravity);
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
            if (m_upOrDown == false)
            {
                m_timerToHeld += dt;
                if (m_timerToHeld > m_maxTimeToHeld)
                {
                    m_upOrDown = true;
                }
            }
            else
            {
                m_timerToHeld -= dt;
                if (m_timerToHeld < 0.0f)
                {
                    m_upOrDown = false;
                }
            }

            Vector2f center = m_selectedPHEnt->GetPosition();

            Vector2f dir = mousePos - center;
            dir.Normalize();

            float distance = m_timerToHeld * (m_facteurPower * 0.3f);

            m_viseur.a = center;
            m_viseur.b = center - dir * distance;
        }
    }
    if (im->IsMouseUp(SDL_BUTTON_LEFT))
    {
        if (m_selectedPHEnt)
        {
            Vector2f center = m_selectedPHEnt->GetPosition();

            Vector2f dir = mousePos - center;
            dir.Normalize();

            float distance = m_timerToHeld * m_facteurPower;
            m_selectedPHEnt->SetVelocity(m_selectedPHEnt->GetVelocity() - dir * distance);

            m_selectedPHEnt->SetVelocity(m_selectedPHEnt->GetVelocity() - dir * distance);
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
    Debug::DrawRectangle(m_rectUp.position, m_rectUp.size, m_rectUp.color, m_rectUp.filled);
    //down
    Debug::DrawRectangle(m_rectBottom.position, m_rectBottom.size, m_rectBottom.color, m_rectBottom.filled);
    //left
    Debug::DrawRectangle(m_rectLeft.position, m_rectLeft.size, m_rectLeft.color, m_rectLeft.filled);
    //right
    Debug::DrawRectangle(m_rectRight.position, m_rectRight.size, m_rectRight.color, m_rectRight.filled);

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
