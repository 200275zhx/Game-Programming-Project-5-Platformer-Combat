#include "GateActuator.h"

GateActuator::GateActuator(glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision, Gate* gate, int id)
	: StaticAnimatedObject(
		Utility::load_texture(GATEACTUATOR_FILEPATH),
		position,
		radian,
		rotate_direction,
		scale,
		GATEACTUATOR_SEQUENCES,
		15,					// animation col
		1,					// animation row
		1.0f,				// width
		1.0f,				// height
		debug_collision,
		glm::vec3(0.0f),	// texture offset
		1.0f,				// texture width ratio
		false				// is collidable
	), gate_actuator_state(GATEACTUATOR_DEACTIVATED_IDLE), gate(gate), id(id) {}

//GateActuator::GateActuator(glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision, Scene* gate_level, int gate_index, int id)
//	: StaticAnimatedObject(
//		Utility::load_texture(GATEACTUATOR_FILEPATH),
//		position,
//		radian,
//		rotate_direction,
//		scale,
//		GATEACTUATOR_SEQUENCES,
//		15,					// animation col
//		1,					// animation row
//		1.0f,				// width
//		1.0f,				// height
//		debug_collision,
//		glm::vec3(0.0f),	// texture offset
//		1.0f,				// texture width ratio
//		false				// is collidable
//	), gate_actuator_state(GATEACTUATOR_DEACTIVATED_IDLE), gate(nullptr), id(id), gate_level(gate_level), gate_index(gate_index) {}

void GateActuator::update(float delta_time) {
	switch (gate_actuator_state)
	{
	case GATEACTUATOR_DEACTIVATED_IDLE:
		m_animation_indices = m_animation_sequences[GATEACTUATOR_DEACTIVATED_IDLE];
		loop_play(delta_time / 6);
		break;
	case GATEACTUATOR_ACTIVATE:
		m_animation_indices = m_animation_sequences[GATEACTUATOR_ACTIVATE];
		play_once(delta_time);
		if (is_current_animation_complete) {
			if (gate) gate->open();
			set_current_state(GATEACTUATOR_ACTIVATED_IDLE);
		}
		break;
	case GATEACTUATOR_ACTIVATED_IDLE:
		m_animation_indices = m_animation_sequences[GATEACTUATOR_ACTIVATED_IDLE];
		loop_play(delta_time / 6);
		break;
	case GATEACTUATOR_DEACTIVATE:
		m_animation_indices = m_animation_sequences[GATEACTUATOR_DEACTIVATE];
		play_once(delta_time);
		if (is_current_animation_complete) {
			set_current_state(GATEACTUATOR_DEACTIVATED_IDLE);
		}
		break;
	default:
		break;
	}
}