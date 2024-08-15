#pragma once
#include "Utility.h"

struct CollisionBoxInfo {
	float width;
	float height;
	glm::vec3 position;
};

class CollisionDebugBox {
public:
	CollisionDebugBox(GLuint textureID);
	void update(glm::vec3 position, glm::vec3 scale, glm::vec3 size);
	void render(ShaderProgram* program);

private:
	glm::mat4 model_matrix;
	GLuint textureID;
};