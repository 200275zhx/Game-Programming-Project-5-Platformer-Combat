#pragma once
#include "StaticAnimatedObject.h"

constexpr char	GATE_FILEPATH[] = "assets/Sprites/Animated-Objects/Door/door open 41x48.png",
				GATE_OPEN_SFX[] = "assets/Sound/SFX/Object SFX/gate open.wav",
				GATE_CLOSE_SFX[] = "assets/Sound/SFX/Object SFX/col cage open.wav";

const std::vector<std::vector<int>> GATE_SEQUENCES {
	{0, 1, 2, 3},							// IDLE
	{4, 5, 6, 7, 8, 9, 10, 11, 12, 13},		// OPEN
	{13},									// OPEN IDLE
	{13, 7, 6, 5, 4}						// CLOSE
};

enum GateState {
	GATE_IDLE,
	GATE_OPEN,
	GATE_OPEN_IDLE,
	GATE_CLOSE
};

class Gate : public StaticAnimatedObject {
public:
	Gate(glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, bool debug_collision, int id);
	void update(float delta_time) override;
	void open() { is_open = true; }
	void set_current_state(GateState state) {
		m_animation_index = 0;
		m_animation_indices = m_animation_sequences[state];
		gate_state = state;
	}
	const ObjectType	get_type()				const override { return GATE; }
	const bool          get_is_interactive()    const override { return false; }

	const int			get_id()				const { return id; }

private:
	bool is_open;
	int id;
	GateState gate_state;
};