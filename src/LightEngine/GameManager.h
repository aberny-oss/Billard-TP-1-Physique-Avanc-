#pragma once

#include <list>

#include "Color.h"
#include "Utils.h"
#include "Rendering.h"

class Entity;
class Scene;
class Debug;

struct SDL_Window;
struct SDL_Renderer;
struct TTF_TextEngine;
struct TTF_Font;

class GameManager
{
    std::list<Entity*> mEntities;
    std::list<Entity*> mEntitiesToAdd;

    std::list<Line*> m_lines;
    std::list<Rectangle*> m_rectangles;

    SDL_Window* mpWindow;
    SDL_Renderer* mpRenderer;
    TTF_TextEngine* mpTextEngine;
    TTF_Font* mpFont;

    Scene* mpScene;

    float mDeltaTime;

    const float m_fixedDt = 1 / 60.0f;
    float m_acc = 0.f;

    int mWindowWidth;
    int mWindowHeight;

    Color mClearColor;

private:
    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    bool Initialize(int width, int height, const char* title, Color clearColor);
    void Run();
    void Update();
    void UpdatePhysics();
    void Draw();

    void SetDeltaTime(uint64 deltaTimeNS) { mDeltaTime = static_cast<float>(deltaTimeNS) / 1000000000.f; }

    SDL_Window* GetWindow() const { return mpWindow; }

    void ResolveCollisionWithLine(Entity* p_entity);
    bool ResolveColisionCBCWithFixed(Entity* p_entA, Entity* p_entB);
    void ResolveCollisionARC(Entity* p_entA);
    bool ResolveColisionCBC(Entity* p_entA, Entity* p_entB);
    bool ResolveCollisonContinueCBC(Entity* p_entA, Entity* p_entB);

public:
    ~GameManager();
    static GameManager* Get();

    SDL_Renderer* GetRenderer() const { return mpRenderer; }
    TTF_TextEngine* GetTextEngine() const { return mpTextEngine; }
    TTF_Font* GetFont() const { return mpFont; }

    template<typename T>
    void LaunchScene(int width = 1280, int height = 720, const char* title = "LightEngine", Color clearColor = Color::Black);

    float GetDeltaTime() const { return mDeltaTime; }
    Scene* GetScene() const { return mpScene; }

    void CreateLine(Line& p_line);
    void CreateRectangle(Rectangle& p_rectangle);

    void FixeUpdate(float dt);

    friend class Scene;
};

#include "GameManager.inl"
