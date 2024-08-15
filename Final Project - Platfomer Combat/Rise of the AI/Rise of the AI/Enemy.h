#pragma once
#include "Entity.h"

class Character;

enum EnemyType {
    NORMIE,
    TARNISHED_WIDOW
};

class Enemy : public Entity {
public:
    Enemy();
	Enemy(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power,
        std::vector<std::vector<int>> animation_sequences, float animation_time,
        int animation_index, int animation_cols, int animation_rows, float width, float height,
        int texture_width_ratio);

    virtual void ai_activate(Entity* player, float delta_time) = 0;
    virtual void ai_take_damage(Entity* player, float damage) = 0;
    virtual const EnemyType get_type() const = 0;
    virtual void check_player_attack(Character* player);
    void update(float delta_time, Entity* player, Map* map);

    const bool get_is_harmless()            const { return is_harmless; }
    const bool get_is_complex_collision()   const { return is_complex_collision; }
    const bool get_is_enemy_take_damage_played()    const { return is_enemy_take_damage_played; }
    const bool get_is_death_played()                const { return is_death_played; }
    const bool get_is_attack_played()       const { return is_attack_played; }
    void set_is_harmless(bool harmless) { is_harmless = harmless; }
    void set_is_enemy_take_damage_played(bool take_damage_played) { is_enemy_take_damage_played = take_damage_played; }
    void set_is_death_played(bool death_played) { is_death_played = death_played; }
    void set_is_attack_played(bool attack_played) { is_attack_played = attack_played; }

protected:
    bool is_damageable = true;
    bool m_is_active = true;
    bool is_harmless = false;
    bool is_complex_collision = false;
    bool is_enemy_take_damage_played = true;
    bool is_death_played = true;
    bool is_attack_played = true;
};