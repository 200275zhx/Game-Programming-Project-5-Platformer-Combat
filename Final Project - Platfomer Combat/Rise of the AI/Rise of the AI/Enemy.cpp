#include "Enemy.h"
#include "Character.h"

Enemy::Enemy()
    : Entity()
{
}

Enemy::Enemy(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power,
    std::vector<std::vector<int>> animation_sequences, float animation_time,
    int animation_index, int animation_cols, int animation_rows, float width, float height,
    int texture_width_ratio)

    : Entity(texture_id, speed, acceleration, jump_power, animation_sequences, animation_time, 
        animation_index, animation_cols, animation_rows, width, height, texture_width_ratio)
{
    m_animation_indices = m_animation_sequences[0];
}

void Enemy::check_player_attack(Character* player) {
    CharacterAction player_action = player->get_current_action();
    switch (player_action)
    {
    case ATTACK_1:
        CollisionBoxInfo player_attack1_collision = player->get_attack_collision_info(IDLE_ATTACK);
        if (check_collision(player_attack1_collision.position, player_attack1_collision.width, player_attack1_collision.height)) {
            if (is_damageable) {
                ai_take_damage(player, 1.0f);
                is_enemy_take_damage_played = false;
            }
            return;
        }
        break;
    case ATTACK_2:
        CollisionBoxInfo player_attack2_collision = player->get_attack_collision_info(DASH_ATTACK);
        if (check_collision(player_attack2_collision.position, player_attack2_collision.width, player_attack2_collision.height)) {
            player->set_is_hit(true);
            if (is_damageable) {
                ai_take_damage(player, 1.0f);
                is_enemy_take_damage_played = false;
            }
            return;
        }
        break;
    case ATTACK_3:
        CollisionBoxInfo player_attack3_collision = player->get_attack_collision_info(JUMP_ATTACK);
        if (check_collision(player_attack3_collision.position, player_attack3_collision.width, player_attack3_collision.height)) {
            player->reset_double_jump();
            if (is_damageable) {
                ai_take_damage(player, 1.0f);
                player->impulse_up(7.0f);
                is_enemy_take_damage_played = false;
            }
            return;
        }
        break;
    default:
        break;
    }
}

void Enemy::update(float delta_time, Entity* player, Map* map)
{
    if (!m_is_active) return;

    ai_activate(player, delta_time);
    check_player_attack(static_cast<Character*>(player));

    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;

    // --------- UPDATE PHYSICS ---------- //

    m_velocity.x = m_movement.x * m_speed;
    m_velocity += m_acceleration * delta_time;

    m_position.y += m_velocity.y * delta_time;
    check_collision_y(map);

    m_position.x += m_velocity.x * delta_time;
    check_collision_x(map);

    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position + m_texture_offset);
    if (m_acceleration == GRAVITY) {
        if (m_velocity.x < 0) { m_direction = -1; }
        if (m_velocity.x > 0) { m_direction = 1; }
        if (m_direction < 0) m_model_matrix = glm::rotate(m_model_matrix, (float)3.1415926f, glm::vec3(0.0f, -1.0f, 0.0f));
    }
    m_model_matrix = glm::rotate(m_model_matrix, m_rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    m_model_matrix = glm::scale(m_model_matrix, m_scale);
    
    //loop_play(delta_time);

    if (m_is_check_collision) collision_box->update(m_position, m_scale, glm::vec3(m_width, m_height, 1.0f));
}