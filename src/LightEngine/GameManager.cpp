#include "GameManager.h"

#include "Entity.h"
#include "Debug.h"
#include "InputManager.h"

#include <string>
#include <cassert>

#include "SDL3/SDL.h"
#include <SDL_ttf.h>

#include <algorithm>
#include <stdlib.h>


GameManager::GameManager()
{
    mpWindow = nullptr;
    mpRenderer = nullptr;
    mpTextEngine = nullptr;
    mpFont = nullptr;
    mDeltaTime = 0.0f;
    mpScene = nullptr;
    mWindowWidth = -1;
    mWindowHeight = -1;
    mClearColor = Color::Black;
    srand(NULL);
}

GameManager* GameManager::Get()
{
    static GameManager mInstance;

    return &mInstance;
}

void GameManager::CreateLine(Line& p_line)
{
    m_lines.push_back(&p_line);
}

void GameManager::CreateRectangle(Rectangle& p_rectangle)
{
    m_rectangles.push_back(&p_rectangle);
}

//void GameManager::FixeUpdate(float dt)
//{
//    for (Entity* ent : mEntities)
//    {
//        ent->FixeUpdate(dt);
//    }
//
//    for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
//    {
//        Entity* entity = *it1;
//
//        if (entity->IsFixed() == false)
//        {
//            ResolveCollisionWithLine(entity);
//            ResolveCollisionARC(entity);
//        }
//
//        auto it2 = it1;
//        ++it2;
//        for (; it2 != mEntities.end(); ++it2)
//        {
//            Entity* otherEntity = *it2;
//
//            if (entity->IsFixed() || otherEntity->IsFixed())
//            {
//                if (ResolveColisionCBCWithFixed(entity, otherEntity) == false)
//                {
//                    continue;
//                }
//            }
//            else
//            {
//                if (ResolveCollisonContinueCBC(entity, otherEntity) == false)
//                {
//                    continue;
//                }
//                //if (ResolveColisionCBC(entity, otherEntity) == false)
//                //{
//                //    continue;
//                //}
//            }
//
//            entity->OnCollision(otherEntity);
//            otherEntity->OnCollision(entity);
//        }
//    }
//}

void GameManager::ResolveCollisionWithLine(Entity* p_entity)
{
    for (Line* line : m_lines)
    {
        Vector2f toLine = line->point - p_entity->GetPosition();
        float distance = Vector2f::Dot(toLine, line->normal);
        float overlap = p_entity->GetRadius() - distance;
        if (overlap > 0.0f)
        {
            p_entity->SetPosition(p_entity->GetPosition() - line->normal * overlap);

            float dotVel = Vector2f::Dot(p_entity->m_velocity, line->normal);
            Vector2f vn = line->normal * dotVel;
            Vector2f vt = p_entity->m_velocity - vn;
            p_entity->m_velocity = -1 * vn + vt;
        }
    }
}

bool GameManager::ResolveColisionCBCWithFixed(Entity* p_entA, Entity* p_entB)
{
    Entity* mouvEnt = nullptr;
    Entity* fixedEnt = nullptr;
    if (p_entB->m_velocity != Vector2f{ 0, 0 })
    {
        mouvEnt = p_entB;
        fixedEnt = p_entA;
    }
    else if (p_entA->m_velocity != Vector2f{ 0, 0 })
    {
        mouvEnt = p_entA;
        fixedEnt = p_entB;
    }

    if (mouvEnt == nullptr || fixedEnt == nullptr)
    {
        return false;
    }
    
    //Correction Position de A en mouvement et B fixe
    Vector2f AB = fixedEnt->GetPosition() - mouvEnt->GetPosition();
    float distance = AB.Magnitude();
    AB.Normalize();
    Vector2f normal = AB;
    float overlap = mouvEnt->GetRadius() + fixedEnt->GetRadius() - distance;
    if (overlap > 0.0f) // collision
    {
        Vector2f newPos = { mouvEnt->GetPosition().x - normal.x * overlap, mouvEnt->GetPosition().y - normal.y * overlap };
        mouvEnt->SetPosition(newPos);

        //Correction Velocité de A en mouvement et B fixe
        Vector2f vn = normal * Vector2f::Dot(mouvEnt->m_velocity, normal);
        Vector2f vt = mouvEnt->m_velocity - vn;
        mouvEnt->m_velocity = -1 * vn + vt;
        return true;
    }
    return false;
}

void GameManager::ResolveCollisionARC(Entity* p_entA)
{
    Vector2f centerA = p_entA->GetPosition();
    float radiusA = p_entA->GetRadius();

    Vector2f topLeft = p_entA->GetPosition(0, 0);
    Vector2f bottomRight = p_entA->GetPosition(1, 1);

    float facteurdeglissement = 0.4f;

    for (Rectangle* rect : m_rectangles)
    {
        Vector2f bottomRightRect = rect->position + rect->size;
        // si rentre dans le if alors il est a l'interieur du rect

        // Améliorer en vérifiant la distance
        // avec le claused point si il renvoie la pos du cercle alors il est dans le rect

        if (topLeft.x >= rect->position.x && topLeft.y >= rect->position.y && bottomRight.x <= bottomRightRect.x && bottomRight.y <= bottomRightRect.y)
        {
            float leftSide = centerA.x - rect->position.x;
            float rightSide = bottomRightRect.x - centerA.x;
            float topSide = centerA.y - rect->position.y;
            float bottomSide = bottomRightRect.y - centerA.y;

            float SideSelected = std::min({ leftSide, rightSide, topSide, bottomSide });

            Vector2f normal = { 0, 0 };
            if (SideSelected == leftSide)
            {
                normal = { 1, 0 };
            }
            else if (SideSelected == rightSide)
            {
                normal = { -1, 0 };
            }
            else if (SideSelected == topSide)
            {
                normal = { 0, 1 };
            }
            else if (SideSelected == bottomSide)
            {
                normal = { 0, -1 };
            }

            float overlap = radiusA + SideSelected;
            p_entA->SetPosition(p_entA->GetPosition() - normal * overlap);

            float dotVel = Vector2f::Dot(p_entA->m_velocity, normal);
            Vector2f vn = normal * dotVel;
            Vector2f vt = p_entA->m_velocity - vn;
            p_entA->m_velocity = -1 * (vn * facteurdeglissement) + vt;

        }
        else
        {
            float closestX = std::clamp(centerA.x, rect->position.x, bottomRightRect.x);
            float closestY = std::clamp(centerA.y, rect->position.y, bottomRightRect.y);
            Vector2f closestPoint = { closestX, closestY };

            Vector2f direction = closestPoint - centerA;
            float distance = direction.Magnitude();

            if (distance >= radiusA) // collision
                continue;

            Vector2f normal = direction.Normalized();
            float overlap = radiusA - distance;
            p_entA->SetPosition(p_entA->GetPosition() - normal * overlap);

            float dotVel = Vector2f::Dot(p_entA->m_velocity, normal);
            Vector2f vn = normal * dotVel;
            Vector2f vt = p_entA->m_velocity - vn;
            p_entA->m_velocity = -1 * (vn * facteurdeglissement) + vt;
        }
    }
}

bool GameManager::ResolveColisionCBC(Entity* p_entA, Entity* p_entB)
{
    Vector2f AB = p_entB->GetPosition() - p_entA->GetPosition();
    float distance = AB.Magnitude();
    Vector2f normal = AB.Normalized();
    float overlap = p_entA->GetRadius() + p_entB->GetRadius() - distance;
    if (overlap > 0.0f) // collision
    {
        float demiOverlap = overlap * 0.5f;
        Vector2f newPosA = { p_entA->GetPosition().x - normal.x * demiOverlap, p_entA->GetPosition().y - normal.y * demiOverlap };
        p_entA->SetPosition(newPosA);

        Vector2f newPosB = { p_entB->GetPosition().x + normal.x * demiOverlap, p_entB->GetPosition().y + normal.y * demiOverlap };
        p_entB->SetPosition(newPosB);


        Vector2f VelocityRelativeBA = p_entA->m_velocity - p_entB->m_velocity;
        float rapprochement = Vector2f::Dot(VelocityRelativeBA, normal);

        //pour éviter de les rapprocher si il s'écarte déja car rapprochement est <= a 0
        if (rapprochement <= 0.0f)
        {
            return true;
        }

        //vector stockant la modification de la velocité de la collision a ajouter a la vélocité de l'objet
        Vector2f CollisionVelA = { 0, 0 };
        Vector2f CollisionVelB = { 0, 0 };

        float massTotal = p_entA->m_mass + p_entB->m_mass;
        float ratioA = (p_entA->m_mass / massTotal);
        float ratioB = 1.f - ratioA;

        //étape 1 anuller le raprochement
        CollisionVelA -= normal * (rapprochement * (1 - ratioA));
        CollisionVelB += normal * (rapprochement * (1 - ratioB));

        float totalRestitution = p_entA->m_restitution + p_entB->m_restitution;
        float moyenne = totalRestitution / 2;

        //ajouter la velocité d'éloignement de moitier pour chacun
        CollisionVelA = { CollisionVelA.x * (1 + moyenne), CollisionVelA.y * (1 + moyenne) };
        CollisionVelB = { CollisionVelB.x * (1 + moyenne), CollisionVelB.y * (1 + moyenne) };

        p_entA->m_velocity += CollisionVelA;
        p_entB->m_velocity += CollisionVelB;

        return true;
    }
    return false;
}

//bool GameManager::ResolveCollisonContinueCBC(Entity* p_entA, Entity* p_entB)
//{
//    Vector2f MouvEntAThisFrame = p_entA->m_velocity * m_fixedDt;
//    Vector2f posA = p_entA->GetPosition();
//    Vector2f normal = (MouvEntAThisFrame - posA).Normalized();
//    Vector2f posB = p_entB->GetPosition();
//    float radiusAB = p_entB->GetRadius() + p_entA->GetRadius();
//    Vector2f AB = posB - posA;
//    // represente la hauteur entre le centre du point b et le point a 90° le plus proche sur le segment du mouvement de A sur cette frame
//    float disCenterBtoMouvA = Vector2f::Dot(AB, normal);
//    if (disCenterBtoMouvA > radiusAB) // collision
//    {
//        return false;
//    }
//
//    float overlap = std::sqrt((radiusAB * radiusAB) - (disCenterBtoMouvA * disCenterBtoMouvA));
//
//    // distance pour la quelle la collision commence car on enleve à la distance du centre de b vers le segment a le décalage pour trouver le debut du chevochement
//    float distancetocollide = Vector2f::Dot(AB, normal) - overlap;
//    float lengthSegmentMouvThisFrame = MouvEntAThisFrame.Magnitude();
//
//    if (0 < distancetocollide && distancetocollide <= MouvEntAThisFrame.Magnitude())
//    {
//        // en gros la proportion de distance a effectué pour que la collision est lieu sur la distance totale du segment de déplacement cette frame
//        float ratioDisToCollide = distancetocollide / lengthSegmentMouvThisFrame;
//
//        float dtImpact = ratioDisToCollide * m_fixedDt; // avance dans le temps (time skip yes i speak english good) donc m_fixeDT - dtImpact = temps restant au passage
//
//        p_entA->SetPosition(posA + p_entA->m_velocity * dtImpact); // on le mouv jusqu'au contacte
//        posA = p_entA->GetPosition();
//
//        Vector2f VelocityRelativeBA = p_entA->m_velocity - p_entB->m_velocity;
//        float rapprochement = Vector2f::Dot(VelocityRelativeBA, normal);
//
//        //pour éviter de les rapprocher si il s'écarte déja car rapprochement est <= a 0
//        if (rapprochement <= 0.0f)
//        {
//            return true;
//        }
//
//        //vector stockant la modification de la velocité de la collision a ajouter a la vélocité de l'objet
//        Vector2f CollisionVelA = { 0, 0 };
//        Vector2f CollisionVelB = { 0, 0 };
//
//        float massTotal = p_entA->m_mass + p_entB->m_mass;
//        float ratioA = (p_entA->m_mass / massTotal);
//        float ratioB = 1.f - ratioA;
//
//        //étape 1 anuller le raprochement
//        CollisionVelA -= normal * (rapprochement * (1 - ratioA));
//        CollisionVelB += normal * (rapprochement * (1 - ratioB));
//
//        float totalRestitution = p_entA->m_restitution + p_entB->m_restitution;
//        float moyenne = totalRestitution / 2;
//
//        //ajouter la velocité d'éloignement de moitier pour chacun
//        CollisionVelA = { CollisionVelA.x * (1 + moyenne), CollisionVelA.y * (1 + moyenne) };
//        CollisionVelB = { CollisionVelB.x * (1 + moyenne), CollisionVelB.y * (1 + moyenne) };
//
//        p_entA->m_velocity += CollisionVelA;
//        p_entB->m_velocity += CollisionVelB;
//
//        //puis en dernier terminer le déplacement restant
//        float dtRestant = m_fixedDt - dtImpact;
//        p_entA->SetPosition(posA + p_entA->m_velocity * dtRestant);
//
//        return true;
//    }
//    return false;
//}

void GameManager::ResolveCCD(Entity* p_entA, Entity* p_entB, float p_impactDt)
{
    p_entA->SetPosition(p_entA->GetPosition() + p_impactDt * p_entA->m_velocity);
    p_entB->SetPosition(p_entB->GetPosition() + p_impactDt * p_entB->m_velocity);
}

GameManager::~GameManager()
{
    while (mEntities.empty() == false)
    {
        Entity* entity = mEntities.front();
        mEntities.pop_front();

        delete entity;
    }

    while (mEntitiesToAdd.empty() == false)
    {
        Entity* entity = mEntitiesToAdd.front();
        mEntitiesToAdd.pop_front();

        delete entity;
    }

    delete mpScene;

    TTF_CloseFont(mpFont);
    TTF_DestroyRendererTextEngine(mpTextEngine);
    TTF_Quit();

    SDL_DestroyRenderer(mpRenderer);
    SDL_DestroyWindow(mpWindow);
    SDL_Quit();
}

bool GameManager::Initialize(int width, int height, const char* title, Color clearColor)
{
    assert(mpWindow == nullptr);
    if (mpWindow != nullptr)
        return false;

    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return false;
    }

    if (SDL_CreateWindowAndRenderer(title, width, height, 0, &mpWindow, &mpRenderer) == false)
    {
        SDL_Log("Window and renderer creation failed: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() == false)
    {
        SDL_Log("SDL_ttf initialization failed: %s", SDL_GetError());
        return false;
    }

    mpTextEngine = TTF_CreateRendererTextEngine(mpRenderer);
    if (mpTextEngine == nullptr)
    {
        SDL_Log("Text renderer creation failed: %s", SDL_GetError());
        return false;
    }

    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr)
    {
        SDL_Log("Application path unavailable: %s", SDL_GetError());
        return false;
    }

    const std::string fontPath = std::string(basePath) + "res/LightEngine/Fonts/Abel-Regular.ttf";
    mpFont = TTF_OpenFont(fontPath.c_str(), 20.f);
    if (mpFont == nullptr)
    {
        SDL_Log("Font loading failed: %s", SDL_GetError());
        return false;
    }

    if (SDL_SetRenderVSync(mpRenderer, 1) == false)
    {
        SDL_Log("VSync activation failed: %s", SDL_GetError());
        return false;
    }

    if (SDL_SetRenderDrawBlendMode(mpRenderer, SDL_BLENDMODE_BLEND) == false)
    {
        SDL_Log("Draw blend mode setup failed: %s", SDL_GetError());
        return false;
    }

    mWindowWidth = width;
    mWindowHeight = height;
    mClearColor = clearColor;

    return true;
}

void GameManager::Run()
{
    InputManager* input = InputManager::Get();

    uint64 previousTime = SDL_GetTicksNS();
    while (input->IsQuit() == false)
    {
        const uint64 frameStart = SDL_GetTicksNS();
        SetDeltaTime(frameStart - previousTime);
        previousTime = frameStart;

        input->Update();

        Update();

        UpdatePhysics();

        Draw();
    }
}

void GameManager::Update()
{
    mpScene->OnUpdate();

    mEntities.splice(mEntities.end(), mEntitiesToAdd);

    //Update
    for (Entity* entity : mEntities)
    {
        if (entity->ToDestroy() == false)
            entity->Update();
    }

    for (auto it = mEntities.begin(); it != mEntities.end(); )
    {
        Entity* entity = *it;

        if (entity->ToDestroy() == false)
        {
            ++it;
            continue;
        }

        delete entity;
        it = mEntities.erase(it);
    }
}

void GameManager::UpdatePhysics()
{
    float dt = GetDeltaTime();
    m_acc += dt;
    while (m_acc >= m_fixedDt)
    {
        USE_CCD ? FixedUpdateCCD() : FixedUpdateDCD();
        m_acc -= m_fixedDt;
    }
}

void GameManager::FixedUpdateDCD()
{
    for (Entity* ent : mEntities)
    {
        ent->FixeUpdate(m_fixedDt);
    }

    for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
    {
        Entity* entity = *it1;

        if (entity->IsFixed() == false)
        {
            ResolveCollisionWithLine(entity);
            ResolveCollisionARC(entity);
        }

        auto it2 = it1;
        ++it2;
        for (; it2 != mEntities.end(); ++it2)
        {
            Entity* otherEntity = *it2;

            if (entity->IsFixed() || otherEntity->IsFixed())
            {
                if (ResolveColisionCBCWithFixed(entity, otherEntity) == false)
                {
                    continue;
                }
            }
            else
            {
                if (ResolveColisionCBC(entity, otherEntity) == false)
                {
                    continue;
                }
            }

            entity->OnCollision(otherEntity);
            otherEntity->OnCollision(entity);
        }
    }
}

void GameManager::FixedUpdateCCD()
{
    float remainingDt = m_fixedDt;

    while (remainingDt > 0)
    {
        bool found = false;

        for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
        {
            Entity* entity = *it1;

            if (entity->IsFixed() == false)
            {
                ResolveCollisionWithLine(entity);
                ResolveCollisionARC(entity);
            }

            auto it2 = it1;
            ++it2;
            for (; it2 != mEntities.end(); ++it2)
            {
                Entity* otherEntity = *it2;

                float impactDT = entity->WillCollide(otherEntity, remainingDt);

                if (impactDT < 0.f)
                    continue;

                found = true;

                ResolveCCD(entity, otherEntity, impactDT);

                entity->Bounce(otherEntity);

                entity->OnCollision(otherEntity);
                otherEntity->OnCollision(entity);

                remainingDt -= impactDT;
            }
        }

        if (found == false)
        {
            for (Entity* ent : mEntities)
            {
                ent->FixeUpdate(remainingDt);
            }
            break;
        }

    }

    
}

void GameManager::Draw()
{
    SDL_SetRenderDrawColor(mpRenderer,
        mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
    SDL_RenderClear(mpRenderer);

    Debug::Draw(mpRenderer, mpTextEngine, mpFont, 0);

    for (Entity* entity : mEntities)
    {
        entity->Draw(mpRenderer);
    }

    Debug::Draw(mpRenderer, mpTextEngine, mpFont, 1);

    Debug::Clear();

    SDL_RenderPresent(mpRenderer);
}
