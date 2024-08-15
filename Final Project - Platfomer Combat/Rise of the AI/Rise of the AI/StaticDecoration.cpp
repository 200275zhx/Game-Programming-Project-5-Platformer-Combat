#include "StaticDecoration.h"

StaticDecoration::StaticDecoration(GLuint textureID, glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision) 
	: StaticAnimatedObject(
		textureID,
		position,
		radian,
		rotate_direction,
		scale,
		{{}},
		1,					// animation col
		1,					// animation row
		1.0f,				// width
		1.0f,				// height
		debug_collision,
		glm::vec3(0.0f),	// texture offset
		1.0f,				// texture width ratio
		false				// is collidable
	) {}