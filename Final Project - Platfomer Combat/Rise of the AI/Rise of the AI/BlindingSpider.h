#pragma once
#include "Enemy.h"

const std::vector<std::vector<int>> BLINDINGSPIDER_SEQUENCE{
	{0}
};

enum BlindingSpiderState {
	BLINDINGSPIDER_WALK
};

class BlindingSpider : public Enemy {
public:
	BlindingSpider(GLuint textureID);
	void ai_activate(Entity* player, float delta_time) override;
	void ai_take_damage(Entity* player, float damage) override {}
	const EnemyType get_type() const override { return NORMIE; }

private:
	BlindingSpiderState state;
	float walk_direction;
};