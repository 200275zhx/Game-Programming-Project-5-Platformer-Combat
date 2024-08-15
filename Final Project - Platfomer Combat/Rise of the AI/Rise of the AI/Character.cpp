#include "Character.h"
#include <cmath>
#include "Enemy.h"
#include "GateActuator.h"
#include "TarnishedWidow.h"

Character::Character(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power,
    std::vector<std::vector<int>> animation_sequences, float animation_time,
    int animation_index, int animation_cols, int animation_rows, float width, float height)

    : Entity(texture_id, speed, acceleration, jump_power, animation_sequences, animation_time,
        animation_index, animation_cols, animation_rows, width, height
        //, PLAYER
    ), character_movement_state(INAIR), current_action(IDLE), hit_back_animation_timer(0.0f),
    is_on_hit(false), is_movable(true), is_dashable(true), in_air_dash_count(1), 
    is_take_damage(false), double_jump_count(MAX_DOUBLE_JUMP_TIMES)
{
    GLuint debug_collision_box_texture_id = Utility::load_texture(COLLISION_DEBUG_BOX_FILEPATH);
    collision_box_attack1 = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_attack2 = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_attack3 = new CollisionDebugBox(debug_collision_box_texture_id);
    is_current_animation_complete = false;
}

CollisionBoxInfo Character::find_closest_collision(const glm::vec3& position, const std::vector<CollisionBoxInfo>& collision_info,
    int collision_info_begin, int collision_info_end) {

    CollisionBoxInfo closest_collision = collision_info[collision_info_begin];
    float min_distance = glm::distance(position, closest_collision.position);

    for (int i{ collision_info_begin }; i < collision_info_end; ++i) {
        float distance = glm::distance(position, collision_info[i].position);
        if (distance < min_distance) {
            closest_collision = collision_info[i];
            min_distance = distance;
        }
    }

    return closest_collision;
}

void Character::on_hit(const std::vector<Enemy*>& enemies, 
    float hit_back_power, float delta_time, float hit_back_duration, float invulnerable_duration) {
    if (!m_is_active) return;
    
    Enemy* collidable_entity = nullptr;

    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i]->get_is_active()) {
            collidable_entity = enemies[i];
            if (is_on_hit) break;
            if (collidable_entity->get_is_harmless()) continue;

            if (!collidable_entity->get_is_complex_collision()) {
                if (check_collision(collidable_entity)) {
                    glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - collidable_entity->get_position()));
                    hit_back = hit_back_dir * hit_back_power;
                    is_on_hit = true;
                    is_take_damage = true;
                    is_movable = false;
                    break;
                }
            }
            else {
                if (collidable_entity->get_type() == TARNISHED_WIDOW) {
                    TarnishedWidow* tarnished_widow = static_cast<TarnishedWidow*>(collidable_entity);
                    std::vector<CollisionBoxInfo> collision_info = tarnished_widow->get_collision_box_info();
                    set_tarnished_widow_closest_collision(find_closest_collision(m_position, collision_info, 0, 4));
                    switch (tarnished_widow->get_current_state())
                    {
                    case TARNISHEDWIDOW_ATTACK:
                        if (tarnished_widow->get_animation_index() == 5 || tarnished_widow->get_animation_index() == 6)
                            if (check_collision(collision_info[4].position, collision_info[4].width, collision_info[4].height)) {
                                //std::cout << "on-hit\n";
                                glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - collision_info[4].position));
                                hit_back = hit_back_dir * hit_back_power;
                                is_on_hit = true;
                                is_take_damage = true;
                                is_movable = false;
                            }
                        if (check_collision(tarnished_widow_closest_collision.position, tarnished_widow_closest_collision.width, tarnished_widow_closest_collision.height)) {
                            //std::cout << "on-hit\n";
                            glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - tarnished_widow_closest_collision.position));
                            hit_back = hit_back_dir * hit_back_power;
                            is_on_hit = true;
                            is_take_damage = true;
                            is_movable = false;
                        }
                        break;
                    case TARNISHEDWIDOW_SPIT_ATTACK:
                        if (tarnished_widow->get_animation_index() == 5 || tarnished_widow->get_animation_index() == 6)
                            if (check_collision(collision_info[5].position, collision_info[5].width, collision_info[5].height)) {
                                //std::cout << "on-hit\n";
                                glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - collision_info[5].position));
                                hit_back = hit_back_dir * hit_back_power;
                                is_on_hit = true;
                                is_take_damage = true;
                                is_movable = false;
                            }
                        if (check_collision(tarnished_widow_closest_collision.position, tarnished_widow_closest_collision.width, tarnished_widow_closest_collision.height)) {
                            //std::cout << "on-hit\n";
                            glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - tarnished_widow_closest_collision.position));
                            hit_back = hit_back_dir * hit_back_power;
                            is_on_hit = true;
                            is_take_damage = true;
                            is_movable = false;
                        }
                        break;
                    case TARNISHEDWIDOW_JUMP_ATTACK_LAND:
                        if (tarnished_widow->get_animation_index() == 1 || tarnished_widow->get_animation_index() == 2) {
                            if (check_collision(collision_info[6].position, collision_info[6].width, collision_info[6].height)) {
                                //std::cout << "on-hit\n";
                                glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - collision_info[6].position));
                                hit_back = hit_back_dir * hit_back_power;
                                is_on_hit = true;
                                is_take_damage = true;
                                is_movable = false;
                            }
                        }
                        if (check_collision(tarnished_widow_closest_collision.position, tarnished_widow_closest_collision.width, tarnished_widow_closest_collision.height)) {
                            //std::cout << "on-hit\n";
                            glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - tarnished_widow_closest_collision.position));
                            hit_back = hit_back_dir * hit_back_power;
                            is_on_hit = true;
                            is_take_damage = true;
                            is_movable = false;
                        }
                        break;
                    case TARNISHEDWIDOW_BUFF_ATTACK:
                        if (tarnished_widow->get_animation_index() == 6 || tarnished_widow->get_animation_index() == 7 || tarnished_widow->get_animation_index() == 8)
                            if (check_collision(collision_info[7].position, collision_info[7].width, collision_info[7].height)) {
                                //std::cout << "on-hit\n";
                                glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - collision_info[7].position));
                                hit_back = hit_back_dir * hit_back_power;
                                is_on_hit = true;
                                is_take_damage = true;
                                is_movable = false;
                            }
                        if (check_collision(tarnished_widow_closest_collision.position, tarnished_widow_closest_collision.width, tarnished_widow_closest_collision.height)) {
                            //std::cout << "on-hit\n";
                            glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - tarnished_widow_closest_collision.position));
                            hit_back = hit_back_dir * hit_back_power;
                            is_on_hit = true;
                            is_take_damage = true;
                            is_movable = false;
                        }
                        break;
                    default:
                        if (check_collision(tarnished_widow_closest_collision.position, tarnished_widow_closest_collision.width, tarnished_widow_closest_collision.height)) {
                            //std::cout << "on-hit\n";
                            glm::vec3 hit_back_dir = glm::normalize(glm::vec3(m_position - tarnished_widow_closest_collision.position));
                            hit_back = hit_back_dir * hit_back_power;
                            is_on_hit = true;
                            is_take_damage = true;
                            is_movable = false;
                        }
                        break;
                    }
                }
            }
        }
    }

    if (is_on_hit && collidable_entity) {
        hit_back_animation_timer += delta_time;
        if (hit_back_animation_timer < hit_back_duration) {
            m_movement = hit_back * delta_time / (1 + hit_back_duration - hit_back_animation_timer) * glm::vec3(200.0f);
        }
        if (hit_back_animation_timer > hit_back_duration) {
            is_movable = true;
        }
        if (hit_back_animation_timer > hit_back_duration + invulnerable_duration) {
            is_on_hit = false;
            hit_back_animation_timer = 0;
        }
    }

    if (is_take_damage) {
        --life;
        std::cout << "character damage taken: " << 1 << ", current life: " << life << std::endl;
        set_current_action(TAKE_DAMAGE);
        is_take_damage = false;
    }
}

void Character::update(float delta_time, const std::vector<Enemy*>& enemies, const std::vector<StaticAnimatedObject*>& objects, Map* map)
{
    if (life == 0) deactivate();
    if (!m_is_active) return;

    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;

    collision_box_attack1->update(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    collision_box_attack2->update(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    collision_box_attack3->update(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));

    std::pair<std::vector<Enemy*>, Enemy*> nearby_enemies = enemies_around(enemies, 10.0f);

    // --------- UPDATE MOVEMENT ---------- //

    if (current_action == ATTACK_1) m_movement.x = 0;
    if (current_action == DASH || current_action == ATTACK_2) m_movement.x = m_direction * 0.0001f;
    //if (current_action != DASH && current_action != ATTACK_3 && !nearby_enemies.first.empty()) on_hit(nearby_enemies.first, 1.0f, delta_time, 0.1f, 0.5f);
    if (current_action != DASH && !nearby_enemies.first.empty()) on_hit(nearby_enemies.first, 1.0f, delta_time, 0.1f, 0.5f); // attack 3???

    if (wall_jump_direction) {
        if (current_action != JUMPING) { wall_jump_direction = 0; }
        m_velocity.x = wall_jump_direction * m_speed;
    }
    else { m_velocity.x = m_movement.x * m_speed; }
    m_velocity += m_acceleration * delta_time;

    if (current_action == WALL_SLIDE) m_position.y += -1.0f * delta_time;
    else m_position.y += m_velocity.y * delta_time;
    m_position.x += m_velocity.x * delta_time;
    check_collision_y(map);
    check_collision_x(map);
    check_collision_objects(objects);

    //std::cout << "collision l, r, t, b: (" << m_collided_left << ", "
    //    << m_collided_right << ", " << m_collided_top << ", " <<
    //    m_collided_bottom << ")\n";

    // --------- UPDATE MOVEMENT STATE ---------- //

    prev_state = character_movement_state;

    if (m_collided_bottom) character_movement_state = GROUNDED;
    else if (m_collided_left || m_collided_right) character_movement_state = ONWALL;
    else character_movement_state = INAIR;

    // --------- UPDATE ACTION ---------- //

    prev_action = current_action;

    if (current_action != JUMP_START && current_action != DASH && current_action != TAKE_DAMAGE)
    switch (character_movement_state)
    {
    case GROUNDED:
        m_texture_offset = glm::vec3(0.0f, 0.0f, 0.0f);
        if (prev_state == INAIR) current_action = LANDING;
        double_jump_count = MAX_DOUBLE_JUMP_TIMES;
        if (current_action != DASH && current_action != ATTACK_2) is_dashable = true;
        break;
    case INAIR:
        m_texture_offset = glm::vec3(0.0f, 0.0f, 0.0f);
        if (is_current_animation_complete && m_velocity.y <= 0 && current_action != DOUBLE_JUMP_START && current_action != ATTACK_3) 
            current_action = m_velocity.y > -1.0 ? FALL_START : FALLING;
        if (m_is_attacking) {
            if (current_action != ATTACK_3 && is_current_animation_complete) {
                set_current_action(ATTACK_3);
                m_velocity.y -= m_jumping_power;
            }
        }
        break;
    case ONWALL:
        if (prev_state == INAIR) { m_velocity.y = 0.0f; }
        if (!is_wall_jump) current_action = WALL_SLIDE;
        else current_action = JUMPING;
        break;
    default:
        break;
    }

    // --------- UPDATE ANIMATION ---------- //

    switch (current_action)
    {
    case IDLE:
        m_animation_indices = m_animation_sequences[IDLE];
        loop_play(delta_time);
        if (m_velocity.x != 0) current_action = RUNING;
        if (m_is_attacking) set_current_action(ATTACK_1);
        break;
    case IDLE_EXTRA:
        break;
    case RUNING:
        m_animation_indices = m_animation_sequences[RUNING];
        loop_play(delta_time);
        if (m_velocity.x == 0) current_action = IDLE;
        if (m_is_attacking) {
            set_current_action(ATTACK_1);
        }
        break;
    case JUMP_START:
        m_animation_indices = m_animation_sequences[JUMP_START];
        play_once(delta_time);
        if (is_current_animation_complete) {
            current_action = JUMPING;
            m_velocity.y = m_jumping_power;
        }
        break;
    case JUMPING:
        m_animation_indices = m_animation_sequences[JUMPING];
        if (is_wall_jump) {
            m_velocity.y = m_jumping_power;
            if (m_collided_left) wall_jump_direction = 1;
            if (m_collided_right) wall_jump_direction = -1;
            is_wall_jump = false;
        }
        loop_play(delta_time);
        if (m_velocity.y < 1.0f) { 
            wall_jump_direction = 0;
            current_action = JUMP_END; 
        }
        break;
    case JUMP_END:
        m_animation_indices = m_animation_sequences[JUMP_END];
        play_once(delta_time);
        if (m_velocity.y < 0.0f) { current_action = FALL_START; }
        break;
    case FALL_START:
        m_animation_indices = m_animation_sequences[FALL_START];
        play_once(delta_time);
        if (m_velocity.y < -1.0f) { current_action = FALLING; }
        break;
    case FALLING:
        m_animation_indices = m_animation_sequences[FALLING];
        loop_play(delta_time);
        if (character_movement_state == GROUNDED) { set_current_action(LANDING); }
        break;
    case LANDING:
        ground_level = m_position.y;
        if (is_tarnished_widow_second_phase) m_reflection_enabled = true;
        //std::cout << "ground level update to: " << m_position.y << std::endl;
        is_dashable = true;
        m_animation_indices = m_animation_sequences[LANDING];
        play_once(delta_time);
        if (is_current_animation_complete) set_current_action(IDLE);
        break;
    case DOUBLE_JUMP_START:
        m_animation_indices = m_animation_sequences[DOUBLE_JUMP_START];
        play_once(delta_time);
        if (double_jump_count > 0) {
            m_velocity.y = m_jumping_power;
            --double_jump_count;
        }
        if (is_current_animation_complete) { current_action = DOUBLE_JUMP_ROLL; }
        break;
    case DOUBLE_JUMP_ROLL:
        m_animation_indices = m_animation_sequences[DOUBLE_JUMP_ROLL];
        play_once(delta_time);
        if (is_current_animation_complete) {
            current_action = DOUBLE_JUMP_TOP;
        }
        break;
    case DOUBLE_JUMP_TOP:
        m_animation_indices = m_animation_sequences[DOUBLE_JUMP_TOP];
        loop_play(delta_time);
        if (m_velocity.y < -1.0f) {
            current_action = DOUBLE_JUMP_END;
        }
        break;
    case DOUBLE_JUMP_END:
        m_animation_indices = m_animation_sequences[DOUBLE_JUMP_END];
        play_once(delta_time);
        if (is_current_animation_complete) {
            current_action = FALL_START;
        }
        break;
    case WALL_GRAB:
        break;
    case WALL_SLIDE:
        m_texture_offset = glm::vec3(-0.2f * m_direction, 0.0f, 0.0f);
        m_animation_indices = m_animation_sequences[WALL_SLIDE];
        m_velocity.y = 0.0f;
        play_once(delta_time);
        if (character_movement_state == GROUNDED) current_action = IDLE;
        if (character_movement_state == INAIR) current_action = FALLING;
        break;
    case FLOOR_SLIDE:
        break;
    case DASH:
        is_dashable = false;
        if (character_movement_state == INAIR) {
            m_movement.x = 0.0f;
            m_position.y -= m_velocity.y * delta_time;
            m_velocity.y = 0.0f;
        }
        m_animation_indices = m_animation_sequences[DASH];
        if (!m_collided_left && ! m_collided_right) m_position += glm::vec3(m_direction, 0.0f, 0.0f) * delta_time * 10.0f;
        play_once(delta_time * 2);
        if (is_current_animation_complete) {
            if (character_movement_state == INAIR) is_dashable = false;
            else if (character_movement_state == GROUNDED) is_dashable = true;
            set_current_action(IDLE);
        }
        if (m_is_attacking && character_movement_state == GROUNDED) set_current_action(ATTACK_2);
        break;
    case TAKE_DAMAGE:
        is_movable = false;
        m_animation_indices = m_animation_sequences[TAKE_DAMAGE];
        play_once(delta_time);
        if (is_current_animation_complete) {
            is_movable = true;
            set_current_action(IDLE);
        }
        break;
    case ATTACK_1:
        if (m_is_check_collision) {
            collision_box_attack1->update(
                m_position + glm::vec3(m_direction / 3.0f, -0.25f, 0.0f),       // position
                m_scale,                                                        // scale
                glm::vec3(0.3f, 0.5f, 1.0f)                                     // size (width and height)
            );
        }
        set_attack_collision_info(IDLE_ATTACK, { 0.3f, 0.5f, m_position + glm::vec3(m_direction / 3.0f, -0.25f, 0.0f) });
        m_animation_indices = m_animation_sequences[ATTACK_1];
        play_once(delta_time * 2);
        if (is_current_animation_complete) {
            set_current_action(IDLE);
            set_attack_collision_info(IDLE_ATTACK, { 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f) });
            m_is_attacking = false;
        }
        break;
    case ATTACK_2:
        if (m_is_check_collision) {
            collision_box_attack2->update(
                m_position + glm::vec3(m_direction / 2.0f, -0.25f, 0.0f),       // position
                m_scale,                                                        // scale
                glm::vec3(0.3f, 0.2f, 1.0f)                                     // size (width and height)
            );
        }
        set_attack_collision_info(DASH_ATTACK, { 0.3f, 0.2f, m_position + glm::vec3(m_direction / 2.0f, -0.25f, 0.0f) });
        m_animation_indices = m_animation_sequences[ATTACK_2];
        if (!m_collided_left && !m_collided_right) m_position += glm::vec3(m_direction, 0.0f, 0.0f) * delta_time * 5.0f;
        play_once(delta_time);
        if (is_current_animation_complete || is_hit) {
            set_current_action(ATTACK2_TO_IDLE);
            set_attack_collision_info(DASH_ATTACK, { 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f) });
            m_is_attacking = false;
            is_dashable = true;
            if (is_hit) is_hit = false;
        }
        break;
    case ATTACK_3:
        if (m_is_check_collision) {
            collision_box_attack3->update(
                m_position + glm::vec3(m_direction / 5.0f, -0.5f, 0.0f),        // position
                m_scale,                                                        // scale
                glm::vec3(0.7f, 0.3f, 1.0f)                                     // size (width and height)
            );
        }
        set_attack_collision_info(JUMP_ATTACK, { 0.7f, 0.3f, m_position + glm::vec3(m_direction / 5.0f, -0.5f, 0.0f) });
        m_animation_indices = m_animation_sequences[ATTACK_3];
        play_once(delta_time);
        if (character_movement_state == GROUNDED) { set_current_action(ATTACK_3_LANDING); }
        break;
    case ATTACK_3_LANDING:
        m_animation_indices = m_animation_sequences[ATTACK_3_LANDING];
        play_once(delta_time);
        if (is_current_animation_complete) {
            current_action = IDLE;
            set_attack_collision_info(JUMP_ATTACK, { 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f) });
            m_is_attacking = false;
        }
        break;
    case DUCKING:
        break;
    case CROUCH_IDLE:
        break;
    case CROUCH_WALK:
        break;
    case ATTACK2_TO_IDLE:
        m_animation_indices = m_animation_sequences[ATTACK2_TO_IDLE];
        play_once(delta_time);
        if (is_current_animation_complete) {
            current_action = IDLE;
        }
        break;
    default:
        break;
    }

    // --------- UPDATE TRANSFORMATION ---------- //

    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position + m_texture_offset);
    if (current_action != TAKE_DAMAGE) {
        if (m_velocity.x < 0) { m_direction = -1; }
        if (m_velocity.x > 0) { m_direction = 1; }
    }
    // Attack Aid
    if (current_action == ATTACK_1 && nearby_enemies.second) { m_direction = m_position.x - nearby_enemies.second->get_position().x > 0.0f ? -1 : 1; }

    if (m_direction < 0) m_model_matrix = glm::rotate(m_model_matrix, (float)3.1415926f, glm::vec3(0.0f, -1.0f, 0.0f));
    m_model_matrix = glm::scale(m_model_matrix, m_scale);

    // --------- UPDATE REFLECTION ---------- //

    if (m_reflection_enabled) {
        m_reflection_offset = (m_position.y + m_texture_offset.y - ground_level) * 2.0f + 1.0f;
    }

    // --------- UPDATE DEBUG COLLISION ---------- //

    if (m_is_check_collision) collision_box->update(m_position, m_scale, glm::vec3(m_width, m_height, 1.0f));
}

std::pair<std::vector<Enemy*>, Enemy*> Character::enemies_around(const std::vector<Enemy*>& enemies, float detection_distance) const {
    std::vector<Enemy*> detected_enemies{};
    std::pair<Enemy*, float> closest_enemy = { nullptr, 100.0f };
    for (Enemy* enemy : enemies) {
        if (enemy->get_is_active()) {
            float enemy_distance = glm::distance(m_position, enemy->get_position());
            if (enemy_distance < detection_distance) {
                detected_enemies.push_back(enemy);
                if (closest_enemy.second > enemy_distance) {
                    closest_enemy.first = enemy;
                    closest_enemy.second = enemy_distance;
                }
            }
        }
    }
    return { detected_enemies, closest_enemy.first };
}

void Character::render_debug(ShaderProgram* program) const { 
    if (!m_is_check_collision) { return; }
    collision_box->render(program);
    switch (current_action)
    {
    case ATTACK_1:
        collision_box_attack1->render(program);
        break;
    case ATTACK_2:
        collision_box_attack2->render(program);
        break;
    case ATTACK_3:
        collision_box_attack3->render(program);
        break;
    default:
        break;
    }
}

void const Character::jump() {
    switch (character_movement_state)
    {
    case GROUNDED:
        current_action = JUMP_START;
        break;
    case INAIR:
        current_action = double_jump_count > 0 ? DOUBLE_JUMP_START : current_action;
        break;
    case ONWALL:
        is_wall_jump = true;
        break;
    default:
        break;
    }
    m_is_attacking = false;
}

std::pair<std::string, std::pair<int, int>> Character::interact(StaticAnimatedObject* item) {
    if (!item) return { "", { -1, -1 } };
    if (item->get_type() == GATE_ACTUATOR) {
        GateActuator* gate_actuator = static_cast<GateActuator*>(item);
        if (gate_actuator->get_current_state() == GATEACTUATOR_DEACTIVATED_IDLE) { 
            gate_actuator->set_current_state(GATEACTUATOR_ACTIVATE); 
            return { "gate_actuator", { gate_actuator->get_id(), GATEACTUATOR_ACTIVATE } };
        }
        //else if (gate_actuator->get_current_state() == GATEACTUATOR_ACTIVATED_IDLE) { gate_actuator->set_current_state(GATEACTUATOR_DEACTIVATE); }
    }
    return { "", { -1, -1 } };
}

StaticAnimatedObject* Character::closest_interactive_item(const std::vector<StaticAnimatedObject*>& items, float reactive_distance) const {
    std::pair<StaticAnimatedObject*, float> closest_item = { nullptr, 100.0f };
    for (StaticAnimatedObject* item : items) {
        if (item->get_is_active() && item->get_is_interactive()) {
            float item_distance = glm::distance(m_position, item->get_position());
            if (item_distance < reactive_distance) {
                if (closest_item.second > item_distance) {
                    closest_item.first = item;
                    closest_item.second = item_distance;
                }
            }
        }
    }
    return closest_item.first;
}