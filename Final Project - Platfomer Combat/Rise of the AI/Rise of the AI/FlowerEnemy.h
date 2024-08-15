#pragma once
#include "Enemy.h"

constexpr char  FLOWER_ENEMY_TAKE_DAMAGE_SFX_FILEPATH[] = "assets/Sound/SFX/Enemy SFX/Flower Enemy/enemy damage over time.wav",
				FLOWER_ENEMY_DEATH_SFX_FILEPATH[] = "assets/Sound/SFX/Enemy SFX/Flower Enemy/enemy death sword.wav",
				FLOWER_ENEMY_ATTACK_SFX_FILEPATH[] = "assets/Sound/SFX/Enemy SFX/Flower Enemy/gg blue room orb final boom.wav";

const std::vector<std::vector<int>> FLOWERENEMY_SEQUENCE{
	{0},											// IDLE
	{1, 2, 3, 4, 5},								// WALK
	{6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},	// ATTACK
	{6, 18, 18},									// TAKE DAMAGE
	{6, 19, 20, 20},								// DEATH
	{1, 2, 3, 4, 5}									// GUARD
};

enum FlowerEnemyState {
	FLOWERENEMY_IDLE,
	FLOWERENEMY_WALK,
	FLOWERENEMY_ATTACK,
	FLOWERENEMY_TAKE_DAMAGE,
	FLOWERENEMY_DEATH,
	FLOWERENEMY_GUARD,

	// No Animation: 
	FLOWERENEMY_WALK_BOT,
	FLOWERENEMY_WALK_TOP,
	FLOWERENEMY_WALK_LEFT,
	FLOWERENEMY_WALK_RIGHT
	//FLOWERENEMY_JUMP_ATTACK
};

enum FlowerEnemyType {
	FLOWERENEMY_STANDARD,
	FLOWERENEMY_WALKER
};

class FlowerEnemy : public Enemy {
public:
	FlowerEnemy(GLuint textureID);
	FlowerEnemy(GLuint textureID, FlowerEnemyType type);

	void ai_activate(Entity* player, float delta_time) override;
	void ai_take_damage(Entity* player, float damage) override;
	const EnemyType get_type() const override { return NORMIE; }
	void set_current_state(FlowerEnemyState current_state) {
		prev_state = state;
		m_animation_index = 0;
		state = current_state;
	}

private:
	FlowerEnemyState state;
	FlowerEnemyState prev_state;
	FlowerEnemyType type;
	float life = 10.0f;
	bool is_walker_offset = false;
	//bool is_jump_attack_begin;
};