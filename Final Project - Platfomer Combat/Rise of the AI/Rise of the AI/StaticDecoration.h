#pragma once
#include "StaticAnimatedObject.h"

class StaticDecoration : public StaticAnimatedObject {
public:
	StaticDecoration(GLuint textureID, glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision);
	void update(float delta_time) { return; }

	const int			get_id()				const { return -1; }
	const ObjectType	get_type()				const override { return DECORATION; }
	const bool          get_is_interactive()    const override { return true; }
};