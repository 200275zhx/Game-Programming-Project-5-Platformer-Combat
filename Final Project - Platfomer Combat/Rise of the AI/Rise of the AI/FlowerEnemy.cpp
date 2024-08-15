#include "FlowerEnemy.h"

FlowerEnemy::FlowerEnemy(GLuint textureID) :
	Enemy
    (textureID,                                         // texture id
    1.0f,                                               // speed
    GRAVITY,                                            // acceleration
    9.0f,                                               // jumping power
    FLOWERENEMY_SEQUENCE,                               // animation index sets
    0.0f,                                               // animation time
    0,                                                  // current animation index
    5,                                                  // animation column amount
    5,                                                  // animation row amount
    0.4f,                                               // width
    0.3f,                                               // height
    1.0f                                                // texture size ratio width / height
    ),
    state(FLOWERENEMY_IDLE), type(FLOWERENEMY_STANDARD) {
    set_texture_offset(glm::vec3(0.0f, 0.35f, 0.0f));
}

FlowerEnemy::FlowerEnemy(GLuint textureID, FlowerEnemyType type) :
    Enemy
    (textureID,                                         // texture id
        1.0f,                                               // speed
        GRAVITY,                                            // acceleration
        9.0f,                                               // jumping power
        FLOWERENEMY_SEQUENCE,                               // animation index sets
        0.0f,                                               // animation time
        0,                                                  // current animation index
        5,                                                  // animation column amount
        5,                                                  // animation row amount
        0.4f,                                               // width
        0.3f,                                               // height
        1.0f                                                // texture size ratio width / height
    ),
    state(FLOWERENEMY_IDLE), type(type) {
    set_texture_offset(glm::vec3(0.0f, 0.35f, 0.0f));
}

void FlowerEnemy::ai_activate(Entity* player, float delta_time) {
    if (state != FLOWERENEMY_TAKE_DAMAGE && state != FLOWERENEMY_DEATH) is_damageable = true;
    if (type == FLOWERENEMY_STANDARD)
        switch (state)
        {
        case FLOWERENEMY_IDLE:
            m_animation_indices = m_animation_sequences[FLOWERENEMY_IDLE];
            loop_play(delta_time);
            if (glm::distance(m_position, player->get_position()) < 1.5f) set_current_state(FLOWERENEMY_WALK);
            break;
        case FLOWERENEMY_WALK:
            if (glm::distance(m_position, player->get_position()) < 0.7f) {
                set_current_state(FLOWERENEMY_ATTACK);
                is_attack_played = false;
            }
            if (glm::distance(m_position, player->get_position()) > 4.0f) { set_current_state(FLOWERENEMY_GUARD); }
            m_animation_indices = m_animation_sequences[FLOWERENEMY_WALK];
            loop_play(delta_time);
            if (m_position.x > player->get_position().x) { m_movement = glm::vec3(-1.0f, 0.0f, 0.0f); }
            else { m_movement = glm::vec3(1.0f, 0.0f, 0.0f); }
            break;
        case FLOWERENEMY_ATTACK:
            m_movement.x = 0.0f;
            set_texture_offset(glm::vec3(0.0f, 0.0f, 0.0f));
            m_width = 0.9f;
            m_height = 0.9f;
            m_animation_indices = m_animation_sequences[FLOWERENEMY_ATTACK];
            play_once(delta_time);
            if (is_current_animation_complete) {
                m_width = 0.5f;
                m_height = 0.3f;
                set_texture_offset(glm::vec3(0.0f, 0.35f, 0.0f));
                m_position -= glm::vec3(0.0f, 0.35f, 0.0f);
                set_current_state(FLOWERENEMY_WALK);
            }
            break;
        case FLOWERENEMY_TAKE_DAMAGE:
            if (prev_state == FLOWERENEMY_ATTACK) {
                m_width = 0.5f;
                m_height = 0.3f;
                set_texture_offset(glm::vec3(0.0f, 0.35f, 0.0f));
                m_position -= glm::vec3(0.0f, 0.35f, 0.0f);
                set_current_state(FLOWERENEMY_WALK);
            }
            m_movement.x = 0.0f;
            m_animation_indices = m_animation_sequences[FLOWERENEMY_TAKE_DAMAGE];
            m_position.x -= delta_time * m_direction * 5.0f;                        // this need to change to use m_movement and also solve the facing issue while using m_movement ?!#
            play_once(delta_time);
            if (is_current_animation_complete) {
                if (life <= 0.0f) {
                    is_harmless = true;
                    is_death_played = false;
                    set_current_state(FLOWERENEMY_DEATH);
                }
                else { set_current_state(FLOWERENEMY_WALK); }
            }
            break;
        case FLOWERENEMY_DEATH:
            m_movement.x = 0.0f;
            m_animation_indices = m_animation_sequences[FLOWERENEMY_DEATH];
            play_once(delta_time / 2);
            if (is_current_animation_complete) deactivate();
            break;
        case FLOWERENEMY_GUARD:
            m_animation_indices = m_animation_sequences[FLOWERENEMY_GUARD];
            loop_play(delta_time);
            if (glm::distance(m_position, player->get_position()) < 3.0f) set_current_state(FLOWERENEMY_WALK);
            break;
        default:
            break;
        }

    if (type == FLOWERENEMY_WALKER)
        switch (state)
        {
        case FLOWERENEMY_IDLE:
            m_animation_indices = m_animation_sequences[FLOWERENEMY_IDLE];
            loop_play(delta_time);
            if (m_collided_bottom) { state = FLOWERENEMY_WALK_BOT; m_movement.x = 1.0f; }
            break;
        case FLOWERENEMY_WALK_BOT:
            m_animation_indices = m_animation_sequences[FLOWERENEMY_WALK];
            loop_play(delta_time);
            if (m_collided_bottom) {
                is_walker_offset = true;
            }
            else {
                if (is_walker_offset) {
                    m_position.x += 0.2f;
                    m_position.y -= 0.7f;
                    m_texture_offset = glm::vec3(0.3f, 0.0f, 0.0f);
                    float temp_width = m_width;
                    m_width = m_height;
                    m_height = temp_width;
                    is_walker_offset = false;
                }
                m_movement.x = 0.0f;
                m_acceleration = glm::vec3(GRAVITY.y, 0.0f, 0.0f);
                m_velocity.y = -1.0f;
                set_rotation(-1.5707963f);

                if (m_collided_left) state = FLOWERENEMY_WALK_LEFT;
            }
            break;
        case FLOWERENEMY_WALK_LEFT:
            m_animation_indices = m_animation_sequences[FLOWERENEMY_WALK];
            loop_play(delta_time);
            if (m_collided_left) {
                is_walker_offset = true;
            }
            else {
                if (is_walker_offset) {
                    m_position.y -= 0.2f;
                    m_position.x -= 0.7f;
                    m_texture_offset = glm::vec3(0.0f, -0.3f, 0.0f);
                    float temp_width = m_width;
                    m_width = m_height;
                    m_height = temp_width;
                    is_walker_offset = false;
                }
                m_movement.x = -1.0f;
                m_acceleration = glm::vec3(0.0f, -GRAVITY.y, 0.0f);
                m_velocity.y = 0.0f;
                set_rotation(-3.1415926f);

                if (m_collided_top) state = FLOWERENEMY_WALK_TOP;
            }
            break;
        case FLOWERENEMY_WALK_TOP:
            m_animation_indices = m_animation_sequences[FLOWERENEMY_WALK];
            loop_play(delta_time);
            if (m_collided_top) {
                is_walker_offset = true;
            }
            else {
                if (is_walker_offset) {
                    m_position.x -= 0.2f;
                    m_position.y += 0.7f;
                    m_texture_offset = glm::vec3(-0.3f, 0.0f, 0.0f);
                    float temp_width = m_width;
                    m_width = m_height;
                    m_height = temp_width;
                    is_walker_offset = false;
                }
                m_movement.x = 0.0f;
                m_acceleration = glm::vec3(-GRAVITY.y, 0.0f, 0.0f);
                m_velocity.y = 1.0f;
                set_rotation(1.5707963f);

                if (m_collided_right) state = FLOWERENEMY_WALK_RIGHT;
            }
            break;
        case FLOWERENEMY_WALK_RIGHT:
            m_animation_indices = m_animation_sequences[FLOWERENEMY_WALK];
            loop_play(delta_time);
            if (m_collided_right) {
                is_walker_offset = true;
            }
            else {
                if (is_walker_offset) {
                    m_position.y += 0.2f;
                    m_position.x += 0.7f;
                    m_texture_offset = glm::vec3(0.0f, 0.3f, 0.0f);
                    float temp_width = m_width;
                    m_width = m_height;
                    m_height = temp_width;
                    is_walker_offset = false;
                }
                m_movement.x = 1.0f;
                m_acceleration = glm::vec3(0.0f, GRAVITY.y, 0.0f);
                m_velocity.y = 0.0f;
                set_rotation(0.0f);

                if (m_collided_bottom) state = FLOWERENEMY_WALK_BOT;
            }
            break;
        case FLOWERENEMY_TAKE_DAMAGE:
            break;
        case FLOWERENEMY_DEATH:
            break;
        default:
            break;
        }

    //switch (state) {
    //case FLOWERENEMY_IDLE:
    //    if (glm::distance(m_position, player->get_position()) < 3.0f) {
    //        is_jump_attack_begin = true;
    //        state = FLOWERENEMY_JUMP_ATTACK;
    //    }
    //    break;

    //case FLOWERENEMY_JUMP_ATTACK:
    //    if (is_jump_attack_begin) {
    //        m_position = player->get_position();
    //        m_position.y += 3.0f;
    //        is_jump_attack_begin = false;
    //    }
    //    if (!m_collided_bottom) m_position.y -= 0.1f;
    //    else { state = FLOWERENEMY_IDLE; }
    //    break;

    //default:
    //    break;
    //}
}

void FlowerEnemy::ai_take_damage(Entity* player, float damage) {
    set_current_state(FLOWERENEMY_TAKE_DAMAGE);
    std::cout << "flower enemy damage taken: " << damage << ", current life: " << life << std::endl;
    life -= damage;
    is_damageable = false;
}