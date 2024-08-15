#include "BlindingSpider.h"

BlindingSpider::BlindingSpider(GLuint textureID) :
    Enemy
    (textureID,                                         // texture id
    3.0f,                                               // speed
    GRAVITY,                                            // acceleration
    9.0f,                                               // jumping power
    BLINDINGSPIDER_SEQUENCE,                            // animation index sets
    0.0f,                                               // animation time
    0,                                                  // current animation index
    1,                                                  // animation column amount
    1,                                                  // animation row amount
    1.0f,                                               // width
    1.0f,                                               // height
    1.0f                                                // texture size ratio width / height
    ), state(BLINDINGSPIDER_WALK), walk_direction(-1.0f){}

void BlindingSpider::ai_activate(Entity* player, float delta_time) {
    if (m_collided_left) walk_direction = 1.0f;
    if (m_collided_right) walk_direction = -1.0f;
    m_movement = glm::vec3(walk_direction, 0.0f, 0.0f);
}