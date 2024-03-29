#include "Colliders.h"
#include "ComponentSystem/Components.h"

#include <glm/gtx/norm.hpp>

bool CheckForBoxWithBoxCollision(BoxCollider * b1, BoxCollider * b2) {
    auto b1Pos = b1->entity->get<TransformComponent>()->position;
    auto b2Pos = b2->entity->get<TransformComponent>()->position;

    float min1x = b1Pos.x + b1->_min.x;
    float min1y = b1Pos.y + b1->_min.y;
    float max1x = b1Pos.x + b1->_max.x;
    float max1y = b1Pos.y + b1->_max.y;

    float min2x = b2Pos.x + b2->_min.x;
    float min2y = b2Pos.y + b2->_min.y;
    float max2x = b2Pos.x + b2->_max.x;
    float max2y = b2Pos.y + b2->_max.y;

    return min1x < max2x && min2x < max1x
        && min1y < max2y && min2y < max1y;
}

bool ArePointsInRange(glm::vec2 p1, glm::vec2 p2, float range) {
    return glm::distance2(p1, p2) < range * range;
}

bool CheckForBoxWithCircleCollision(BoxCollider * box, CircleCollider * circle) {
    auto boxPosition = box->entity->get<TransformComponent>()->position;
    auto circlePosition = circle->entity->get<TransformComponent>()->position;

    // collision cases
    // corner |  top   | corner
    // ------------------------
    // left   | inside | right
    // ------------------------
    // corner | bottom | corner
    bool isInCorner =
        (circlePosition.x < boxPosition.x + box->_min.x || circlePosition.x > boxPosition.x + box->_max.x)
        && (circlePosition.y < boxPosition.y + box->_min.y || circlePosition.y > boxPosition.y + box->_max.y);
    if (isInCorner) {
        // circle center is in corner
        // check if the corners are inside the circle
        glm::vec2 circlePos(circlePosition.x, circlePosition.y);
        glm::vec2 boxPos(boxPosition.x, boxPosition.y);
        return ArePointsInRange(circlePos, boxPos + box->_min, circle->radius)
            || ArePointsInRange(circlePos, boxPos + glm::vec2(box->_min.x, box->_max.y), circle->radius)
            || ArePointsInRange(circlePos, boxPos + glm::vec2(box->_max.x, box->_min.y), circle->radius)
            || ArePointsInRange(circlePos, boxPos + box->_max, circle->radius);
    }

    // not in cornet treat the circle as box
    BoxCollider tempBox(circle->entity, -circle->radius, -circle->radius, circle->radius, circle->radius);
    return CheckForBoxWithBoxCollision(&tempBox, box);
}

bool CheckForCircleWithCircleCollision(CircleCollider * c1, CircleCollider * c2) {
    auto c1Pos = c1->entity->get<TransformComponent>()->position;
    auto c2Pos = c2->entity->get<TransformComponent>()->position;
    return ArePointsInRange(glm::vec2(c1Pos.x, c1Pos.y), glm::vec2(c2Pos.x, c2Pos.y), c1->radius + c2->radius);
}
