#pragma once
#include "Entity.h"
#include "StaticAnimatedObject.h"

constexpr char  JUMP_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/brkn wand jump.wav",
				LAND_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/brkn wand land.wav",
				WALK_1_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/geo hit ground 1.wav", 
				WALK_2_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/geo hit ground 2.wav",
				IDLE_ATTACK_1_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/sword 1.wav", 
				IDLE_ATTACK_2_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/sword 2.wav", 
				IDLE_ATTACK_3_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/sword 3.wav", 
				DASH_ATTACK_1_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/sword 4.wav", 
				DASH_ATTACK_2_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/sword 5.wav", 
				JUMP_ATTACK_1_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/zombie shield sword 3.wav", 
				JUMP_ATTACK_2_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/zombie miner throw.wav",
				DASH_1_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/brkn wand down stab dash.wav", 
				//DASH_2_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/brkn wand horizontal dash dash.wav",
				TAKE_DAMAGE_SFX_FILEPATH[] = "assets/Sound/SFX/Player SFX/enemy damage.wav";

class Enemy;

enum CharacterState {
	GROUNDED,
	INAIR,
	ONWALL
};

enum CharacterAction {
	IDLE,
	IDLE_EXTRA,
	RUNING,
	JUMP_START,
	JUMPING,
	JUMP_END,
	FALL_START,
	FALLING,
	LANDING,
	DOUBLE_JUMP_START,
	DOUBLE_JUMP_ROLL,
	DOUBLE_JUMP_TOP,
	DOUBLE_JUMP_END,
	WALL_GRAB,
	WALL_SLIDE,
	FLOOR_SLIDE,
	DASH,
	TAKE_DAMAGE,
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	ATTACK_3_LANDING,
	DUCKING,
	CROUCH_IDLE,
	CROUCH_WALK,
	ATTACK2_TO_IDLE
};

enum AttackType {
	IDLE_ATTACK,
	DASH_ATTACK,
	JUMP_ATTACK
};

class Character : public Entity {
public:
	Character(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power,
		std::vector<std::vector<int>> animation_sequences, float animation_time,
		int animation_index, int animation_cols, int animation_rows, float width, float height);
	~Character() {
		delete collision_box_attack1;
		delete collision_box_attack2;
		delete collision_box_attack3;
	}

	void update(float delta_time, const std::vector<Enemy*>& enemies, const std::vector<StaticAnimatedObject*>& objects, Map* map);
	void render_debug(ShaderProgram* program) const override;

	void set_is_hit(bool hit) { is_hit = hit; }
	void set_is_interacting(bool interacting) { is_interacting = interacting; }
	void reset_double_jump() { double_jump_count = 1; }
	void set_current_action(CharacterAction action) { 
		m_animation_index = 0;
		current_action = action; 
	}
	void set_attack_collision_info(AttackType attack_type, const CollisionBoxInfo& info) { collision_box_info[attack_type] = info; }
	void set_tarnished_widow_closest_collision(const CollisionBoxInfo& info) { tarnished_widow_closest_collision = info; }
	void set_is_movable(bool movable) { is_movable = movable; }
	void set_life(float new_life) { life = new_life; }
	void set_is_tarnished_widow_second_phase(bool second_phase) { is_tarnished_widow_second_phase = second_phase; }

	const CharacterAction	get_current_action()									const { return current_action; }
	const CharacterState	get_current_state()										const { return character_movement_state; }
	const CharacterAction	get_prev_action()									const { return prev_action; }
	const CharacterState	get_prev_state()										const { return prev_state; }
	const bool				movable()												const { return is_movable; }
	const bool				get_is_dashable()										const { return is_dashable; }
	const bool				get_is_interacting()									const { return is_interacting; }
	const bool				get_is_attacking()										const { return m_is_attacking; }
	const bool				get_is_jumpable()										const { return double_jump_count > 0; }
	const float				get_life()												const { return life; }
	const CollisionBoxInfo&	get_attack_collision_info(AttackType attack_type)		const { return collision_box_info[attack_type]; }
	const CollisionBoxInfo& get_tarnished_widow_closest_collision()					const { return tarnished_widow_closest_collision; }

	std::pair<std::vector<Enemy*>, Enemy*> enemies_around(const std::vector<Enemy*>& enemies, float detection_distance) const;
	std::pair<std::string, std::pair<int, int>> interact(StaticAnimatedObject* item);
	StaticAnimatedObject* closest_interactive_item(const std::vector<StaticAnimatedObject*>& items, float reactive_distance) const;
	CollisionBoxInfo find_closest_collision(const glm::vec3& position, const std::vector<CollisionBoxInfo>& collision_info, int collision_info_begin, int collision_info_end);

	void on_hit(const std::vector<Enemy*>& enemies, 
		float hit_back_power, float delta_time, float hit_back_duration, float invulnerable_duration);
	void dash() { if (is_dashable) { current_action = DASH; } }
	void input_interact() { is_interacting = true; }
	void const jump() override;
	void impulse_up(float magnitude) { 
		m_velocity.y = magnitude; 
		set_current_action(JUMPING);
		m_is_attacking = false;
	}

private:
//	float attack_damage;
	CharacterState character_movement_state;
	CharacterState prev_state;
	CharacterAction current_action;
	CharacterAction prev_action;
	float life = 90;
	float hit_back_animation_timer;
	float ground_level;
	bool is_on_hit;
	bool is_movable;
	bool is_dashable;
	bool is_take_damage;
	bool is_wall_jump = false;
	bool is_hit = false;
	bool is_interacting = false;
	bool is_tarnished_widow_second_phase = false;
	int wall_jump_direction = 0;
	int in_air_dash_count;
	int double_jump_count;
	int MAX_DOUBLE_JUMP_TIMES = 1;
	
	glm::vec3 hit_back = glm::vec3(0.0f);

	CollisionDebugBox* collision_box_attack1;
	CollisionDebugBox* collision_box_attack2;
	CollisionDebugBox* collision_box_attack3;
	std::vector<CollisionBoxInfo> collision_box_info = {
		{0.3f, 0.5f, glm::vec3(m_direction / 3.0f, -0.25f, 0.0f)},
		{0.3f, 0.3f, glm::vec3(m_direction / 2.0f, -0.25f, 0.0f)},
		{0.7f, 0.3f, glm::vec3(m_direction / 5.0f, -0.5f, 0.0f)}
	};

	CollisionBoxInfo tarnished_widow_closest_collision = {};
};