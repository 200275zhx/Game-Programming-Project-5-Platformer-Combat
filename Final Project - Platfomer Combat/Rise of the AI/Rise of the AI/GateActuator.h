#pragma once
#include "Gate.h"
#include "Scene.h"

constexpr char GATEACTUATOR_FILEPATH[] = "assets/Sprites/Animated-Objects/Save/save packed.png";

const std::vector<std::vector<int>> GATEACTUATOR_SEQUENCES{
	{0, 1, 2, 3},							// DEACTIVATED IDLE
	{4, 5, 6},								// ACTIVE
	{7, 8, 9, 10},							// ACTIVATED IDLE
	{11, 12, 13, 14}						// DEACTIVATE
};

enum GateActuatorState {
	GATEACTUATOR_DEACTIVATED_IDLE,
	GATEACTUATOR_ACTIVATE,
	GATEACTUATOR_ACTIVATED_IDLE,
	GATEACTUATOR_DEACTIVATE
};

class GateActuator : public StaticAnimatedObject {
public:
	GateActuator(glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision, Gate* gate, int id);
	//GateActuator(glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision, Scene* gate_level, int gate_index, int id);
	void update(float delta_time) override;
	void set_current_state(GateActuatorState state) {
		m_animation_index = 0;
		gate_actuator_state = state;
	}
	const GateActuatorState get_current_state() const { return gate_actuator_state; }
	const int				get_id()			const { return id; }

	const ObjectType	get_type()				const override { return GATE_ACTUATOR; }
	const bool          get_is_interactive()    const override { return true; }

private:
	GateActuatorState gate_actuator_state;
	Gate* gate;
	//Scene* gate_level;
	//int gate_index;
	int id;
};