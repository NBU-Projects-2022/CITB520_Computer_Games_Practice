#pragma once

#include <glm/glm.hpp>
#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Core/Common.h"

struct TransformComponent {
    ECS_DECLARE_TYPE;

    TransformComponent(float positionX, float positionY, float positionZ, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f)
        : position(positionX, positionY, positionZ), rotation(rotation), scale(scaleX, scaleY)
    {}
    TransformComponent(glm::vec3 position, float rotation = 0.0f, glm::vec2 scale = glm::vec2(1.0f))
        : position(position), rotation(rotation), scale(scale)
    {}
    TransformComponent()
        : position(0.0f), rotation(0.0f), scale(1.0f)
    {}

    glm::vec3 position;
    float rotation;
    glm::vec2 scale;
};
ECS_DEFINE_TYPE(TransformComponent);

class Sprite;
struct RenderComponent {
    ECS_DECLARE_TYPE;

    RenderComponent(Ref<Sprite> sprite) : sprite(sprite) {}

    Ref<Sprite> sprite;
};
ECS_DEFINE_TYPE(RenderComponent);

class Collider;
struct ColliderComponent {
    ECS_DECLARE_TYPE;

    ColliderComponent(Ref<Collider> collider)
        : collider(collider)
    {}

    Ref<Collider> collider;
};
ECS_DEFINE_TYPE(ColliderComponent);

struct RigidBodyComponent {
    ECS_DECLARE_TYPE;

    RigidBodyComponent(bool isStatic = false, bool isKinematic = false)
        : isStatic(isStatic), isKinematic(isKinematic),
        velocity(0.0f), acceleration(0.0f)
    {}

    bool isStatic;
    bool isKinematic;

    glm::vec2 velocity;
    glm::vec2 acceleration;
};
ECS_DEFINE_TYPE(RigidBodyComponent);

class NativeScript;
struct NativeScriptComponent {
    ECS_DECLARE_TYPE;

    NativeScriptComponent()
        : nativeScript(nullptr)
    {}

    template<typename T>
    T* Bind() {
        if (!nativeScript.get()) {
            nativeScript = CreateRef<T>();
        }

        return reinterpret_cast<T*>(nativeScript.get());
    }

    template<typename T>
    T* Script() {
        return reinterpret_cast<T*>(nativeScript.get());
    }

    Ref<NativeScript> nativeScript;
    friend class NativeScriptSystem;
};
ECS_DEFINE_TYPE(NativeComponent);
