#pragma once

#include "Rendering.h"
#include "Vector.h"
#include "Color.h"

struct SDL_Renderer;

class Scene;

class Entity
{
protected:
    Circle mCircle;
    int mTag = -1;
    bool mToDestroy = false;
    Vector2f m_velocity;
    bool m_fixed = false;
    Vector2f m_acceleration = {0, 0};
    float m_mass = 1.0f;
    float m_restitution = 1.0f;

public:
    void SetPosition(Vector2f position, float ratioX = 0.5f, float ratioY = 0.5f);
    void SetTag(int tag) { mTag = tag; }
    float GetRadius() const { return mCircle.radius; }

    Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;

    bool IsTag(int tag) const { return mTag == tag; }

    void Destroy();
    bool ToDestroy() const { return mToDestroy; }

    template<typename T>
    T* GetScene() const;

    Scene* GetScene() const;
    float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(float radius, const Color& color);

    void SetVelocity(Vector2f p_velocity);
    Vector2f GetVelocity() { return m_velocity; }

    bool IsPointInside(Vector2f p_point);

    void SetColor(Color p_color) { mCircle.color = p_color; }

    void SetIsFixed(bool p_fixed) { m_fixed = p_fixed; }
    bool IsFixed() { return m_fixed; }

    void AddImpulsion(Vector2f p_impulsion) { m_velocity += p_impulsion; }

    void SetAcceleration(Vector2f p_acceleration) { m_acceleration = p_acceleration; }
    Vector2f GetAcceleration() { return m_acceleration; }

    void SetMass(float p_mass) { m_mass = p_mass; }
    float GetMass() { return m_mass; }

    void SetCoefRestitution(float p_coef) { m_restitution = p_coef; }
    float GetCoefRestitution() { return m_restitution; }

protected:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void OnUpdate() {}
    virtual void OnCollision(Entity* collidedWith) {}
    virtual void OnInitialize() {}
    virtual void OnDestroy() {}

private:
    void Update();
    void Draw(SDL_Renderer* renderer) const;
    void Initialize(float radius, const Color& color);

    void FixeUpdate(float dt);

    float WillCollide(const Entity* other, float dt) const;
    void Bounce(Entity* other);

    friend class GameManager;
    friend class Scene;
};

#include "Entity.inl"
