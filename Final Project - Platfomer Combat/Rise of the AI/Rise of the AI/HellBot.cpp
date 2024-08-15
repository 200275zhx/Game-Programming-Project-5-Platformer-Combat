#include "HellBot.h"

HellBot::HellBot(GLuint textureID) : 
    Enemy
    (textureID,                                         // texture id
    2.0f,                                               // speed
    GRAVITY,                                            // acceleration
    9.0f,                                               // jumping power
    HELLBOT_SEQUENCE,                                   // animation index sets
    0.0f,                                               // animation time
    0,                                                  // current animation index
    1,                                                  // animation column amount
    6,                                                  // animation row amount
    1.0f,                                               // width
    1.0f,                                               // height
    2.0f                                                // texture size ratio width / height
    ), 
    state(HELLBOT_IDLE){}

void HellBot::ai_activate(Entity* player, float delta_time) {
    switch (state) {
    case HELLBOT_IDLE:
        if (glm::distance(m_position, player->get_position()) < 3.0f) state = HELLBOT_GUARD;
        break;

    case HELLBOT_GUARD:
        if (m_position.x > player->get_position().x) {
            m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        else {
            m_movement = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        break;

    default:
        break;
    }
}