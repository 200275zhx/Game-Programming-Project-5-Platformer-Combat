#include "TarnishedWidow.h"
#include "Character.h"

TarnishedWidow::TarnishedWidow(GLuint textureID) :
    Enemy
    (textureID,                                         // texture id
    1.0f,                                               // speed
    GRAVITY,                                            // acceleration
    9.0f,                                               // jumping power
    TARNISHEDWIDOW_SEQUENCES,                           // animation index sets
    0.0f,                                               // animation time
    0,                                                  // current animation index
    11,                                                 // animation column amount
    11,                                                 // animation row amount
    1.0f,                                               // width
    1.0f,                                               // height
    1.0f                                                // texture size ratio width / height
    ),
    state(TARNISHEDWIDOW_IDLE), distance(0.0f), jump_timer(1.0f){
    set_texture_offset(glm::vec3(0.0f, 1.83f, 0.0f));
    set_scale(glm::vec3(15.0f));
    is_complex_collision = true;
    
    GLuint debug_collision_box_texture_id = Utility::load_texture(COLLISION_DEBUG_BOX_FILEPATH);
    collision_box_body          = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_back_leg      = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_front_leg     = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_center_leg    = new CollisionDebugBox(debug_collision_box_texture_id);

    collision_box_attack        = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_spit_attack   = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_land_attack   = new CollisionDebugBox(debug_collision_box_texture_id);
    collision_box_buff_attack   = new CollisionDebugBox(debug_collision_box_texture_id);

    collision_boxes.push_back(collision_box_body       );
    collision_boxes.push_back(collision_box_back_leg   );
    collision_boxes.push_back(collision_box_front_leg  );
    collision_boxes.push_back(collision_box_center_leg );
    collision_boxes.push_back(collision_box_attack     );
    collision_boxes.push_back(collision_box_spit_attack);
    collision_boxes.push_back(collision_box_land_attack);
    collision_boxes.push_back(collision_box_buff_attack);
}

TarnishedWidow::~TarnishedWidow() {
    delete collision_box_body       ;
    delete collision_box_back_leg   ;
    delete collision_box_front_leg  ;
    delete collision_box_center_leg ;

    delete collision_box_attack     ;
    delete collision_box_spit_attack;
    delete collision_box_land_attack;
    delete collision_box_buff_attack;

    collision_boxes.clear();
}

void TarnishedWidow::check_player_attack(Character* player) {
    // player attack range too large ??????!!!!
    if (!player->get_is_attacking()) {
        is_take_damage = false;
        return;
    }
    CharacterAction player_action = player->get_current_action();
    CollisionBoxInfo info = player->get_tarnished_widow_closest_collision();
    if (!info.width || !info.height) return;
    switch (player_action)
    {
    case ATTACK_1:
        CollisionBoxInfo attack1_info = player->get_attack_collision_info(IDLE_ATTACK);
        //std::cout << "tarnished widow info: { " << info.position << ", " << info.width << ", " << info.height
        //    << " }     player position: { " << attack1_info.position << ", " << attack1_info.width << ", " << attack1_info.height << " }\n";
        if (check_collision(info.position, info.width, info.height, attack1_info.position, attack1_info.width, attack1_info.height)) {
            if (is_damageable && !is_take_damage) {
                ai_take_damage(player, 1.0f);
                is_take_damage = true;
                is_tarnished_widow_take_light_damage_1_sfx = true;
            }
            return;
        }
        break;
    case ATTACK_2:
        CollisionBoxInfo attack2_info = player->get_attack_collision_info(DASH_ATTACK);
        //std::cout << "\ntarnished widow info: { " << info.position << ", " << info.width << ", " << info.height
        //    << " }     player position: { " << attack2_info.position << ", " << attack2_info.width << ", " << attack2_info.height << " }\n";
        //std::cout << "tarnished_widow position: " << m_position << "    player position: " << player->get_position() << std::endl;
        if (check_collision(info.position, info.width, info.height, attack2_info.position, attack2_info.width, attack2_info.height)) {
            //std::cout << "\ndash attack hit at animation index: " << m_animation_index <<"\n";
            //std::cout << "tarnished widow info: { " << info.position << ", " << info.width << ", " << info.height
            //<< " }     player position: { " << attack2_info.position << ", " << attack2_info.width << ", " << attack2_info.height << " }\n";
            //std::cout << "tarnished_widow position: " << m_position << "    player position: " << player->get_position() << std::endl;
            player->set_is_hit(true);
            if (is_damageable && !is_take_damage) {
                ai_take_damage(player, 1.0f);
                is_take_damage = true;
                is_tarnished_widow_take_light_damage_2_sfx = true;
            }
            return;
        }
        break;
    case ATTACK_3:
        CollisionBoxInfo attack3_info = player->get_attack_collision_info(JUMP_ATTACK);
        if (check_collision(info.position, info.width, info.height, attack3_info.position, attack3_info.width, attack3_info.height)) {
            player->reset_double_jump();
            if (is_damageable && !is_take_damage) {
                ai_take_damage(player, 1.0f);
                player->impulse_up(7.0f);
                is_take_damage = true;
                is_tarnished_widow_take_heavy_damage_sfx = true;
            }
            return;
        }
        break;
    default:
        break;
    }
}

void TarnishedWidow::update_collision(TarnishedWidowCollisionType type, float width, float height, glm::vec3 position) {
    glm::vec3 real_position = m_position + position;
    if (m_is_check_collision) collision_boxes[type]->update(real_position, m_scale, glm::vec3(width, height, 1.0f));
    set_collision_info(type, { width * m_scale.x, height * m_scale.y, real_position });
}

void TarnishedWidow::ai_activate(Entity* player, float delta_time) {
    if (life == 0.0f) {
        set_current_state(TARNISHEDWIDOW_DEATH);
        //std::cout << "play tarnished_widow_death_sfx\n";
        Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_death_sfx, 0);
        --life;
    } 

    update_collision(C_TARNISHED_WIDOW_ATTACK, 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
    update_collision(C_TARNISHED_WIDOW_SPIT_ATTACK, 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
    update_collision(C_TARNISHED_WIDOW_JUMP_ATTACK, 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
    update_collision(C_TARNISHED_WIDOW_BUFF_ATTACK, 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
    
    //if (life >= 0.0f) std::cout << "boss position: " << m_position << "     player position: " << player->get_position() << std::endl;

    if (is_second_phase_begin && is_current_animation_complete) {
        set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_UP);
        //std::cout << "play tarnished_widow_jump_up_sfx\n";
        Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_jump_up_sfx, 0);
        is_second_phase = true;
        is_second_phase_begin = false;
    }

    prev_state = state;

    switch (state)
    {
    case TARNISHEDWIDOW_IDLE:
        m_movement.x = 0.0f;
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_IDLE];
        //std::cout << "IDLE\n";
        loop_play(delta_time);
        switch (m_animation_index)
        {
        case 0:
            if (is_second_phase) {
                if (second_phase_transfer_idle_counter++ >= SECOND_PHASE_TRANSFER_IDLE_LOOP_COUNT) {
                    is_loop_jump = true;
                    set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_UP);
                    //std::cout << "play tarnished_widow_jump_up_sfx\n";
                    Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_jump_up_sfx, 0);
                }
            }
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;
        case 1:
        case 5:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f , 0.3f, 0.0f));
            break;
        case 2:
        case 4:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.5f, 0.0f));
            break;
        case 3:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.64f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.5f, 0.0f));
            break;
        case 6:
        case 10:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.35f, 0.0f));
            break;
        case 7:
        case 8:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.4f, 0.0f));
            break;
        case 9:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.35f, 0.0f));
            break;

        default:
            break;
        }
        //if (glm::distance(m_position, player->get_position()) < 3.0f) set_current_state(TARNISHEDWIDOW_MOVE);
        break;
    case TARNISHEDWIDOW_MOVE:
        distance = glm::distance(m_position, player->get_position());
        if (distance < 3.0f) {
            set_current_state(TARNISHEDWIDOW_ATTACK);
            is_tarnished_widow_attack_sfx = true;
        }
        if (distance > 4.0f && distance < 6.0f) { 
            set_current_state(TARNISHEDWIDOW_SPIT_ATTACK); 
            is_tarnished_widow_spit_sfx = true;
        }
        if (distance > 6.0f) { 
            set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_UP); 
            //std::cout << "play tarnished_widow_jump_up_sfx\n";
            Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_jump_up_sfx, 0);
        }
        if (abs(m_position.x - player->get_position().x) < 2.5f && player->get_position().y > m_position.y && player->get_position().y - m_position.y < 3.0f) {
            set_current_state(TARNISHEDWIDOW_BUFF_ATTACK);
            is_tarnished_widow_buff_sfx = true;
        }

        
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_MOVE];
        loop_play(delta_time);
        if (m_position.x > player->get_position().x) { m_movement = glm::vec3(-1.0f, 0.0f, 0.0f); }
        else { m_movement = glm::vec3(1.0f, 0.0f, 0.0f); }

        switch (m_animation_index)
        {
        case 0:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            //is_tarnished_widow_walk_2_sfx = true;
            break;
        case 1:
            //is_tarnished_widow_walk_2_sfx = false;
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.2f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.3f, 0.0f));
            break;
        case 2:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.5f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.6f, 0.0f));
            break;
        case 3:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.6f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.0f, 0.7f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.15f, glm::vec3(m_direction * 1.3f, 0.85f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 20.0f, 0.3f, 0.0f));
            break;
        case 4:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.7f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 1.8f, 0.7f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.13f, glm::vec3(m_direction * 1.65f, 0.8f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.13f, 0.1f, glm::vec3(-m_direction / 5.0f, 0.3f, 0.0f));
            break;
        case 5:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.8f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 1.6f, 0.5f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.13f, glm::vec3(m_direction * 1.9f, 0.6f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.13f, 0.1f, glm::vec3(-m_direction / 2.0f, 0.3f, 0.0f));
            break;
        case 6:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 1.6f, 0.2f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.13f, glm::vec3(m_direction * 2.0f, 0.5f, 0.0f));
            //is_tarnished_widow_walk_1_sfx = true;
            break;
        case 7:
            //is_tarnished_widow_walk_1_sfx = false;
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.3f, 0.0f));
            break;
        case 8:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 1.6f, 0.2f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.13f, glm::vec3(m_direction * 2.0f, 0.5f, 0.0f));
            break;
        case 9:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.5f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 1.8f, 0.2f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.13f, glm::vec3(m_direction * 1.9f, 0.5f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.13f, 0.1f, glm::vec3(-m_direction / 3.0f, 0.4f, 0.0f));
            break;
        case 10:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.6f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 1.9f, 0.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.13f, glm::vec3(m_direction * 1.5f, 0.5f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.15f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.6f, 0.0f));
            break;
        case 11:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.7f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.1f, glm::vec3(m_direction * 1.3f, 0.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.15f, 0.1f, glm::vec3(m_direction / 10.0f, 0.6f, 0.0f));
            break;
        case 12:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.5f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;

        default:
            break;
        }

        break;
    case TARNISHEDWIDOW_ATTACK:
        m_movement.x = 0.0f;
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_ATTACK];
        play_once(delta_time);
        if (is_current_animation_complete) {
            distance = glm::distance(m_position, player->get_position());
            if (distance > 3.0f && distance < 6.0f) {
                set_current_state(TARNISHEDWIDOW_SPIT_ATTACK);
                is_tarnished_widow_spit_sfx = true;
            }
            else set_current_state(TARNISHEDWIDOW_MOVE);
        }

        switch (m_animation_index)
        {
        case 0:
        case 18:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;
        case 1:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 8.0f, 1.1f, 0.0f));
            break;
        case 2:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 3.0f, 1.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.5f, 2.0f, 0.0f));
            break;
        case 3:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 3.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.5f, 2.1f, 0.0f));
            break;
        case 4:
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.4f, 2.2f, 0.0f));
            break;
        case 5:
            if (is_tarnished_widow_attack_sfx) {
                //std::cout << "play tarnished_widow_attack_sfx\n";
                Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_attack_sfx, 0);
                is_tarnished_widow_attack_sfx = false;
            }
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(m_direction / 2.0f, 1.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 2.6f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 2.0f, 0.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_ATTACK, 0.15, 0.05f, glm::vec3(m_direction * 3.6f, -0.3f, 0.0f));
            break;
        case 6:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(m_direction / 2.0f, 0.9f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 2.7f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_ATTACK, 0.2, 0.02f, glm::vec3(m_direction * 3.6f, -0.4f, 0.0f));
            break;
        case 7:
        //case 8:
        //case 9:
        //case 10:
        //case 11:
        //case 12:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(m_direction / 2.0f, 1.0f, 0.0f));
            break;
        case 13:
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 2.5f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.12f, 0.1f, glm::vec3(m_direction / 2.1f, 0.3f, 0.0f));
            break;
        case 14:
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 2.4f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 4.0f, 0.3f, 0.0f));
            break;
        case 15:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.21f, 0.1f, glm::vec3(m_direction / 2.2f, 1.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.8f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.15f, 0.1f, glm::vec3(m_direction / 5.5f, 0.3f, 0.0f));
            break;
        case 16:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.08f, glm::vec3(m_direction / 5.0f, 1.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.3f, 0.45f, 0.0f));
            break;
        case 17:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.1f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;

        default:
            break;
        }

        break;
    case TARNISHEDWIDOW_SPIT_ATTACK:
        m_movement.x = 0.0f;
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_SPIT_ATTACK];
        play_once(delta_time);
        if (is_current_animation_complete) set_current_state(TARNISHEDWIDOW_MOVE);

        switch (m_animation_index)
        {
        case 0:
        case 18:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;
        case 1:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 8.0f, 1.1f, 0.0f));
            break;
        case 2:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 3.0f, 1.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.4f, 0.0f));
            break;
        case 3:
        //case 4:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 3.0f, 1.4f, 0.0f));
            break;
        case 5:
            if (is_tarnished_widow_spit_sfx) {
                //std::cout << "play tarnished_widow_spit_sfx\n";
                Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_spit_sfx, 0);
                is_tarnished_widow_spit_sfx = false;
            }
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(m_direction / 2.0f, 1.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.8f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 2.0f, 0.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_SPIT_ATTACK, 0.2, 0.1f, glm::vec3(m_direction * 4.3f, 0.9f, 0.0f));
            break;
        case 6:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(m_direction / 2.0f, 0.9f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_SPIT_ATTACK, 0.2f, 0.1f, glm::vec3(m_direction * 4.5f, 1.0f, 0.0f));
            break;
        case 7:
        //case 8:
        //case 9:
        //case 10:
        //case 11:
        //case 12:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(m_direction / 2.0f, 1.0f, 0.0f));
            break;
        case 13:
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.12f, 0.1f, glm::vec3(m_direction / 2.1f, 0.3f, 0.0f));
            break;
        case 14:
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 4.0f, 0.3f, 0.0f));
            break;
        case 15:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.21f, 0.1f, glm::vec3(m_direction / 2.2f, 1.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.15f, 0.1f, glm::vec3(m_direction / 5.5f, 0.3f, 0.0f));
            break;
        case 16:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.08f, glm::vec3(m_direction / 5.0f, 1.3f, 0.0f));
            break;
        case 17:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.1f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;

        default:
            break;
        }

        break;
    case TARNISHEDWIDOW_JUMP_ATTACK_UP:
        m_movement.x = 0.0f;
        is_damageable = false;
        //std::cout << "JUMP ATTACK UP\n";
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_JUMP_ATTACK_UP];
        play_once(delta_time);
        if (is_current_animation_complete) {
            state = TARNISHEDWIDOW_JUMP_HOLD;
            m_position.y = 100.0f;
        }

        switch (m_animation_index)
        {
        case 0:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            //is_tarnished_widow_jump_up_sfx = true;
            break;
        case 1:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;
        case 2:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.6f, 0.0f));
            break;
        case 3:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.6f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.4f, 0.0f));
            break;
        case 4:
        //case 5:
        //case 6:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.5f, 0.0f));
            break;
        case 7:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.4f, glm::vec3(-m_direction / 10.0f, 2.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.32f, glm::vec3(-m_direction * 2.1f, 1.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.32f, glm::vec3(m_direction * 1.2f, 1.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.3f, glm::vec3(m_direction / 8.0f, 1.3f, 0.0f));
            break;
        case 8:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.0, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.0, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.0f, 0.0f, glm::vec3(0.0f, 100.0f, 0.0f));
            break;

        default:
            break;
        }

        break;
    case TARNISHEDWIDOW_JUMP_HOLD:
        //std::cout << "JUMP ATTACK HOLD\n";
        jump_timer -= delta_time;
        if (is_second_phase) {
            if (is_loop_jump) {
                if (loop_jump_count-- <= 0) {
                    is_loop_jump = false;
                    is_second_phase = false;
                    is_second_phase_transfer_complete = true;
                }
                m_position.x = 12.0f;
                jump_timer = jump_hold_time;
                m_position.y = level_height;
                //std::cout << "play tarnished_widow_land_sfx\n";
                Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_land_sfx, 0);
                set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_LAND);
            }
            else {
                m_position.x = 12.0f;
                jump_timer = jump_hold_time;
                m_position.y = level_height;
                //std::cout << "play tarnished_widow_land_sfx\n";
                Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_land_sfx, 0);
                set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_LAND);
            }
        }
        else {
            if (jump_timer >= 0.2f) m_position.x = player->get_position().x;
            if (jump_timer <= 0.0f) {
                jump_timer = jump_hold_time;
                m_position.y = level_height;
                //std::cout << "play tarnished_widow_land_sfx\n";
                Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_land_sfx, 0);
                set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_LAND);
            }
        }
        break;
    case TARNISHEDWIDOW_JUMP_ATTACK_LAND:
        //std::cout << "JUMP ATTACK LAND\n";
        m_movement.x = 0.0f;
        is_damageable = true;
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_JUMP_ATTACK_LAND];
        play_once(delta_time);
        if (is_current_animation_complete) {
            if (is_second_phase) {
                if (is_loop_jump) {
                    set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_UP);
                    //std::cout << "play tarnished_widow_jump_up_sfx\n";
                    Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_jump_up_sfx, 0);
                }
                else {
                    //std::cout << "play tarnished_widow_roar_sfx\n";
                    Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_roar_sfx, 0);
                    set_current_state(TARNISHEDWIDOW_IDLE);
                }
            }
            else {
                set_current_state(TARNISHEDWIDOW_MOVE);
            }
        }

        switch (m_animation_index)
        {
        case 0:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 3.0f, 4.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 1.8f, 3.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 0.8f, 3.0f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 2.3f, 0.0f));
            break;
        case 1:
            //is_tarnished_widow_land_sfx = true;
            
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.6f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_JUMP_ATTACK, 0.4, 0.06f, glm::vec3(-m_direction / 3.0f, -0.1f, 0.0f));
            break;
        case 2:
            update_collision(C_TARNISHED_WIDOW_JUMP_ATTACK, 0.6, 0.01f, glm::vec3(-m_direction / 3.0f, -0.5f, 0.0f));
            is_land_vfx = true;
            if (is_second_phase_transfer_complete) {
                is_floor_break = true;
                static_cast<Character*>(player)->set_is_tarnished_widow_second_phase(true);
                is_second_phase_transfer_complete = false;
                Mix_PlayChannel(-1, current_scene->get_state().floor_break, 0);
                set_current_state(TARNISHEDWIDOW_FALL);
            }
            break;
        case 3:
            is_land_vfx = false;
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.5f, 0.0f));
            break;
        case 4:
        //case 5:
        //case 6:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.5f, 0.0f));
            break;
        case 7:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.6f, 0.0f));
            break;
        case 8:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.5f, 0.0f));
            break;
        case 9:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            break;

        default:
            break;
        }

        break;
    case TARNISHEDWIDOW_FALL:
        is_land_vfx = false;
        is_damageable = true;
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_FALL];
        fall_cooldown += 0.1f;
        loop_play(delta_time);
        if (m_collided_bottom && fall_cooldown > 2.0f) {
            //std::cout << "boss land! second phase start\n";
            level_height = -37.0f;
            //std::cout << "play tarnished_widow_heavy_land_sfx\n";
            Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_heavy_land_sfx, 0);
            set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_LAND);
        }
        break;
    case TARNISHEDWIDOW_BUFF_ATTACK:
        m_movement.x = 0.0f;
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_BUFF_ATTACK];
        play_once(delta_time);
        if (is_current_animation_complete) {
            if (abs(m_position.x - player->get_position().x) < 2.5f && player->get_position().y > m_position.y && player->get_position().y - m_position.y < 3.0f) {
                set_current_state(TARNISHEDWIDOW_JUMP_ATTACK_UP);
                //std::cout << "play tarnished_widow_jump_up_sfx\n";
                Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_jump_up_sfx, 0);
            }
            else
            set_current_state(TARNISHEDWIDOW_MOVE);
        }

        switch (m_animation_index)
        {
        case 0:
        case 1:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.6f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BACK_LEG, 0.05, 0.12f, glm::vec3(-m_direction * 2.1f, 0.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_FRONT_LEG, 0.06, 0.12f, glm::vec3(m_direction * 1.2f, 0.45f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_CENTER_LEG, 0.14f, 0.1f, glm::vec3(m_direction / 8.0f, 0.3f, 0.0f));
            break;
        case 2:
        //case 3:
        //case 4:
        //case 5:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 0.5f, 0.0f));
            break;
        case 6:
            if (is_tarnished_widow_buff_sfx) {
                //std::cout << "play tarnished_widow_buff_sfx\n";
                Mix_PlayChannel(4, current_scene->get_state().tarnished_widow_buff_sfx, 0);
                is_tarnished_widow_buff_sfx = false;
            }
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.6f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BUFF_ATTACK, 0.25f, 0.13f, glm::vec3(-m_direction / 10.0f, 1.8f, 0.0f));
            break;
        case 7:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.5f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BUFF_ATTACK, 0.25f, 0.15f, glm::vec3(-m_direction / 10.0f, 2.0f, 0.0f));
            break;
        case 8:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            update_collision(C_TARNISHED_WIDOW_BUFF_ATTACK, 0.25f, 0.15f, glm::vec3(-m_direction / 10.0f, 2.0f, 0.0f));
            break;
        case 9:
            update_collision(C_TARNISHED_WIDOW_BODY, 0.2f, 0.1f, glm::vec3(-m_direction / 10.0f, 1.4f, 0.0f));
            break;

        default:
            break;
        }

        break;
    case TARNISHEDWIDOW_DEATH:
        m_movement.x = 0.0f;
        m_animation_indices = m_animation_sequences[TARNISHEDWIDOW_DEATH];
        play_once(delta_time);
        is_damageable = false;
        is_harmless = true;
        break;
    default:
        break;
    }
}

void TarnishedWidow::ai_take_damage(Entity* player, float damage) {
    std::cout << "tarnished widow damage taken: " << damage << ", current life: " << life << std::endl;
    life -= damage;
    if (life <= 15.0f && is_able_transfer_phase) {
        is_second_phase_begin = true;
        is_able_transfer_phase = false;
    }
}

void TarnishedWidow::render_debug(ShaderProgram* program) const {
    if (!m_is_check_collision) { return; }
    collision_box_body->render(program);
    collision_box_back_leg->render(program);
    collision_box_front_leg->render(program);
    collision_box_center_leg->render(program);
    switch (state)
    {
    case TARNISHEDWIDOW_ATTACK:
        collision_box_attack->render(program);
        break;
    case TARNISHEDWIDOW_SPIT_ATTACK:
        collision_box_spit_attack->render(program);
        break;
    case TARNISHEDWIDOW_JUMP_ATTACK_LAND:
        collision_box_land_attack->render(program);
        break;
    case TARNISHEDWIDOW_BUFF_ATTACK:
        collision_box_buff_attack->render(program);
        break;
    default:
        break;
    }
}