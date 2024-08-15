#include "Gate.h"

Gate::Gate(glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision, int id)
	: StaticAnimatedObject(
		Utility::load_texture(GATE_FILEPATH),
		position,
		radian, 
		rotate_direction,
		scale, 
		GATE_SEQUENCES,	
		15,					// animation col
		1,					// animation row
		0.2f,				// width
		1.0f,				// height
		debug_collision,
		glm::vec3(0.0f),	// texture offset
		1.0f,				// texture width ratio
		true				// is collidable
	), gate_state(GATE_IDLE), is_open(false), id(id) {}

void Gate::update(float delta_time) {
	switch (gate_state)
	{
	case GATE_IDLE:
		m_animation_indices = m_animation_sequences[GATE_IDLE];
		loop_play(delta_time / 6);
		if (is_open) {
			set_current_state(GATE_OPEN); 
		}
		break;
	case GATE_OPEN:
		m_animation_indices = m_animation_sequences[GATE_OPEN];
		play_once(delta_time);
		if (is_current_animation_complete) set_current_state(GATE_OPEN_IDLE);
		break;
	case GATE_OPEN_IDLE:
		m_is_collidable = false;
		m_animation_indices = m_animation_sequences[GATE_OPEN_IDLE];
		loop_play(delta_time);
		break;
	case GATE_CLOSE:
		m_is_collidable = true;
		m_animation_indices = m_animation_sequences[GATE_CLOSE];
		play_once(delta_time);
		break;
	default:
		break;
	}
}