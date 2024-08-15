#version 330 core

uniform sampler2D diffuse;
in vec2 texCoordVar;

out vec4 color;

uniform bool isReflection;      // Determines if the current render is for the reflection
uniform float reflectionAlpha;  // Control the transparency of the reflection

void main() {
    vec4 spriteColor = texture(diffuse, texCoordVar);

    if (isReflection) {
        spriteColor.a *= reflectionAlpha;
    }

    color = spriteColor;
}
