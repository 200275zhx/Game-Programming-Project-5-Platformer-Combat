#pragma once
#include "Enemy.h"

const std::vector<std::vector<int>> HELLBOT_SEQUENCE = {
	{0, 1, 2, 3, 4, 5}
};

enum HellBotState {
	HELLBOT_IDLE,
	HELLBOT_GUARD
};

class HellBot : public Enemy {
public:
	HellBot(GLuint texture_id);
	void ai_activate(Entity* player, float delta_time) override;
	void ai_take_damage(Entity* player, float damage) override {}
	const EnemyType get_type() const override { return NORMIE; }

private:
	HellBotState state;
};