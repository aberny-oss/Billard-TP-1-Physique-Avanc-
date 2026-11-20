#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"
#include "PhysicEntity.h"
#include <cmath>
#include <algorithm>
#include <stdlib.h>
#include <LightEngine/Utils.h>

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

    float sizeBorderMore = 1000.0f;

    //rect up
    m_rectUp.position = { topLeft.x, topLeft.y - sizeBorderMore };
    m_rectUp.size = { (float)wWidth, holeRadius + sizeBorderMore };
    m_rectUp.color = Color::Green;
    m_rectUp.filled = true;
    gm->CreateRectangle(m_rectUp);

    //rect down
    m_rectBottom.position = { bottomLeft.x, bottomLeft.y - holeRadius };
    m_rectBottom.size = { (float)wWidth, holeRadius + sizeBorderMore };
    m_rectBottom.color = Color::Green;
    m_rectBottom.filled = true;
    gm->CreateRectangle(m_rectBottom);

    //rect left
    m_rectLeft.position = { topLeft.x - sizeBorderMore, topLeft.y };
    m_rectLeft.size = { holeRadius + sizeBorderMore, (float)wHeight };
    m_rectLeft.color = Color::Green;
    m_rectLeft.filled = true;
    gm->CreateRectangle(m_rectLeft);

    //rect right
    m_rectRight.position = { topRight.x - holeRadius, topRight.y };
    m_rectRight.size = { holeRadius + sizeBorderMore, (float)wHeight };
    m_rectRight.color = Color::Green;
    m_rectRight.filled = true;
    gm->CreateRectangle(m_rectRight);


    float radius = 20;
    float diameter = radius * 2.0f;
    float rowHeight = radius * std::sqrt(3.0f); 
    Vector2f placeFirst = { wWidth * 0.5f, wHeight * 0.4f };
    bool color = false;
    for (int i = 0; i < 1; ++i)
    {
        float startX = placeFirst.x - (i * radius);
        float posY = placeFirst.y - (i * rowHeight);

        for (int j = 0; j < i + 1; ++j)
        {
            float posX = startX + (j * diameter);

            Color colorRand = { (uint8)(std::rand() % 256), (uint8)(std::rand() % 256), (uint8)(std::rand() % 256) };

            PhysicEntity* phent = CreateEntity<PhysicEntity>(radius, colorRand);
            phent->SetPosition({ posX, posY });
            //phent->SetAcceleration(m_gravity);
            phent->SetMass(1);
            phent->SetCoefRestitution(1.0f);
            m_phEnt.push_back(phent);
        }
    }

    //White ball
    PhysicEntity* whiteBall = CreateEntity<PhysicEntity>(33, Color::White);
    whiteBall->SetPosition({wWidth * 0.3f, wHeight * 0.8f });
    whiteBall->SetDestroy(false);
    whiteBall->SetSpawnPoint(whiteBall->GetPosition());
    //whiteBall->SetAcceleration(m_gravity);
    whiteBall->SetMass(1);
    whiteBall->SetCoefRestitution(1.0f);
    m_phEnt.push_back(whiteBall);

    m_viseur.a = { 0, 0 };
    m_viseur.b = { 0, 0 };

    m_selectedPHEnt = whiteBall;
}

void SampleScene::OnUpdate()
{
    float dt = GetDeltaTime();
    float fixedt = GetFixeDeltaTime();

    InputManager* im = InputManager::Get();
    const Vector2f mousePos = im->GetMousePosition();

    if (m_selectedPHEnt)
    {
        Vector2f PlayerToMouse = mousePos - m_selectedPHEnt->GetPosition();

        float facRes = 1.0f;

        Vector2f vel = (PlayerToMouse * facRes) / fixedt;

        m_selectedPHEnt->SetVelocity(vel);
    }
    if (im->IsKeyDown(SDLK_A))
    {
        if (m_selectedPHEnt)
        {
            m_selectedPHEnt->SetPosition(mousePos);
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


    if (m_selectedPHEnt)
    {
        Segment s;
        s.a = { 300, 200 };
        s.b = { 500, 400 };

        Debug::DrawLine(s.a, s.b, Color::White);

        Circle c;
        c.position = m_selectedPHEnt->GetPosition();
        c.radius = m_selectedPHEnt->GetRadius();

        IntersectInfo info = Intersect(c, s);

        if (info.found)
            Debug::DrawCircle(info.point, 5.f, Color::Red, true);
    }
    

}
