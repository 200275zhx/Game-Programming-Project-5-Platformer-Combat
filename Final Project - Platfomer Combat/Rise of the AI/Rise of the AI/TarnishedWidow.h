#pragma once
#include "Enemy.h"
#include "Scene.h"

constexpr char  TW_TAKE_LIGHT_DAMAGE_1_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/breakable pole hit 1.wav",
				TW_TAKE_LIGHT_DAMAGE_2_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/breakable pole hit 2.wav",
				TW_TAKE_HEAVY_DAMAGE_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/sword hit reject.wav",
				TW_WALK_1_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/dung ball bounce 1.wav",
				TW_WALK_2_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/dung ball bounce 2.wav",
				TW_JUMP_UP_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/false knight jump.wav",
				TW_LAND_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/false knight land.wav",
				TW_HEAVY_LAND_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/false knight strike ground.wav",
				TW_ATTACK_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/vine plat land.wav",
				TW_SPIT_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/explosion 1.wav",
				TW_BUFF_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/explosion 4 wet.wav",
				TW_DEATH_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/boss final hit.wav",
				TW_ROAR_SFX_FILEPATH[] = "assets/Sound/SFX/Boss SFX/mage lord onscreen appear.wav",
				FLOOR_BREAK[] = "assets/Sound/SFX/Object SFX/false knight ceiling break.wav";

const std::vector<std::vector<int>> TARNISHEDWIDOW_SEQUENCES{
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},														// IDLE
	{11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},										// MOVE
	{23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41},			// ATTACK
	{42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60},			// SPIT ATTACK
	{61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76},						// JUMP ATTACK UP
	{77, 78, 79, 80, 81, 82, 83, 84, 85, 86},												// JUMP ATTACK LAND
	{87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102},					// BUFF ATTACK
	{103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118},		// DEATH
	{77}																					// FALL
};

enum TarnishedWidowState {
	TARNISHEDWIDOW_IDLE,
	TARNISHEDWIDOW_MOVE,
	TARNISHEDWIDOW_ATTACK,
	TARNISHEDWIDOW_SPIT_ATTACK,
	TARNISHEDWIDOW_JUMP_ATTACK_UP,
	TARNISHEDWIDOW_JUMP_ATTACK_LAND,
	TARNISHEDWIDOW_BUFF_ATTACK,
	TARNISHEDWIDOW_DEATH,
	TARNISHEDWIDOW_FALL,
	TARNISHEDWIDOW_JUMP_HOLD
};

enum TarnishedWidowCollisionType {
	C_TARNISHED_WIDOW_BODY,
	C_TARNISHED_WIDOW_BACK_LEG,
	C_TARNISHED_WIDOW_FRONT_LEG,
	C_TARNISHED_WIDOW_CENTER_LEG,
	C_TARNISHED_WIDOW_ATTACK,
	C_TARNISHED_WIDOW_SPIT_ATTACK,
	C_TARNISHED_WIDOW_JUMP_ATTACK,
	C_TARNISHED_WIDOW_BUFF_ATTACK
};

class TarnishedWidow : public Enemy {
public:
	TarnishedWidow(GLuint textureID);
	~TarnishedWidow();

	void ai_activate(Entity* player, float delta_time) override;
	void ai_take_damage(Entity* player, float damage) override;
	
	void set_current_state(TarnishedWidowState current_state) {
		m_animation_index = 0;
		state = current_state;
	}
	void set_collision_info(TarnishedWidowCollisionType type, const CollisionBoxInfo& info) { collision_box_info[type] = info; }
	void set_is_floor_break(bool floor_break) { is_floor_break = floor_break; }
	void set_is_land_vfx(bool land) { is_land_vfx = land; }
	void set_current_scene(Scene* scene) { current_scene = scene; }
	void update_collision(TarnishedWidowCollisionType type, float width, float height, glm::vec3 position);

	const EnemyType							get_type()												const override { return TARNISHED_WIDOW; }
	const CollisionBoxInfo&					get_collision_info(TarnishedWidowCollisionType type)	const { return collision_box_info[type]; }
	const TarnishedWidowState				get_current_state()										const { return state; }
	const TarnishedWidowState				get_previous_state()									const { return prev_state; }
	const std::vector<CollisionBoxInfo>&	get_collision_box_info()								const { return collision_box_info; }
	const int								get_animation_index()									const { return m_animation_index; }
	const bool								get_is_floor_break()									const { return is_floor_break; }
	const bool								get_is_land_vfx()										const { return is_land_vfx; }
	const float								get_life()												const { return life; }

	const bool get_is_tarnished_widow_take_light_damage_1_sfx() const { return is_tarnished_widow_take_light_damage_1_sfx; }
	const bool get_is_tarnished_widow_take_light_damage_2_sfx() const { return is_tarnished_widow_take_light_damage_2_sfx; }
	const bool get_is_tarnished_widow_take_heavy_damage_sfx  () const { return is_tarnished_widow_take_heavy_damage_sfx  ; }
	const bool get_is_tarnished_widow_walk_1_sfx             () const { return is_tarnished_widow_walk_1_sfx             ; }
	const bool get_is_tarnished_widow_walk_2_sfx             () const { return is_tarnished_widow_walk_2_sfx             ; }
	const bool get_is_tarnished_widow_jump_up_sfx            () const { return is_tarnished_widow_jump_up_sfx            ; }
	const bool get_is_tarnished_widow_land_sfx               () const { return is_tarnished_widow_land_sfx               ; }
	const bool get_is_tarnished_widow_heavy_land_sfx         () const { return is_tarnished_widow_heavy_land_sfx         ; }
	const bool get_is_tarnished_widow_attack_sfx             () const { return is_tarnished_widow_attack_sfx             ; }
	const bool get_is_tarnished_widow_spit_sfx               () const { return is_tarnished_widow_spit_sfx               ; }
	const bool get_is_tarnished_widow_buff_sfx               () const { return is_tarnished_widow_buff_sfx               ; }
	const bool get_is_tarnished_widow_roar_sfx               () const { return is_tarnished_widow_roar_sfx               ; }
	const bool get_is_tarnished_widow_death_sfx              () const { return is_tarnished_widow_death_sfx              ; }

	void set_is_tarnished_widow_take_light_damage_1_sfx(bool tarnished_widow_take_light_damage_1_sfx) { is_tarnished_widow_take_light_damage_1_sfx = tarnished_widow_take_light_damage_1_sfx; }
	void set_is_tarnished_widow_take_light_damage_2_sfx(bool tarnished_widow_take_light_damage_2_sfx) { is_tarnished_widow_take_light_damage_2_sfx = tarnished_widow_take_light_damage_2_sfx; }
	void set_is_tarnished_widow_take_heavy_damage_sfx  (bool tarnished_widow_take_heavy_damage_sfx  ) { is_tarnished_widow_take_heavy_damage_sfx   = tarnished_widow_take_heavy_damage_sfx  ; }
	void set_is_tarnished_widow_walk_1_sfx             (bool tarnished_widow_walk_1_sfx             ) { is_tarnished_widow_walk_1_sfx              = tarnished_widow_walk_1_sfx             ; }
	void set_is_tarnished_widow_walk_2_sfx             (bool tarnished_widow_walk_2_sfx             ) { is_tarnished_widow_walk_2_sfx              = tarnished_widow_walk_2_sfx             ; }
	void set_is_tarnished_widow_jump_up_sfx            (bool tarnished_widow_jump_up_sfx            ) { is_tarnished_widow_jump_up_sfx             = tarnished_widow_jump_up_sfx            ; }
	void set_is_tarnished_widow_land_sfx               (bool tarnished_widow_land_sfx               ) { is_tarnished_widow_land_sfx                = tarnished_widow_land_sfx               ; }
	void set_is_tarnished_widow_heavy_land_sfx         (bool tarnished_widow_heavy_land_sfx         ) { is_tarnished_widow_heavy_land_sfx          = tarnished_widow_heavy_land_sfx         ; }
	void set_is_tarnished_widow_attack_sfx             (bool tarnished_widow_attack_sfx             ) { is_tarnished_widow_attack_sfx              = tarnished_widow_attack_sfx             ; }
	void set_is_tarnished_widow_spit_sfx               (bool tarnished_widow_spit_sfx               ) { is_tarnished_widow_spit_sfx                = tarnished_widow_spit_sfx               ; }
	void set_is_tarnished_widow_buff_sfx               (bool tarnished_widow_buff_sfx               ) { is_tarnished_widow_buff_sfx                = tarnished_widow_buff_sfx               ; }
	void set_is_tarnished_widow_roar_sfx               (bool tarnished_widow_roar_sfx               ) { is_tarnished_widow_roar_sfx                = tarnished_widow_roar_sfx               ; }
	void set_is_tarnished_widow_death_sfx              (bool tarnished_widow_death_sfx              ) { is_tarnished_widow_death_sfx               = tarnished_widow_death_sfx              ; }

	void check_player_attack(Character* player) override;
	void render_debug(ShaderProgram* program) const override;

private:
	Scene* current_scene;
	TarnishedWidowState state;
	TarnishedWidowState prev_state;
	float life = 30.0f;
	float distance;
	float jump_timer;
	float jump_hold_time = 1.0f;
	float fall_cooldown = 0.0f;
	float level_height = -18.0f;
	int loop_jump_count = 2;
	int second_phase_transfer_idle_counter = 0;
	const int SECOND_PHASE_TRANSFER_IDLE_LOOP_COUNT = 11;
	bool is_take_damage = false;
	bool is_second_phase = false;
	bool is_second_phase_begin = false;
	bool is_second_phase_transfer_complete = false;
	bool is_loop_jump = false;
	bool is_floor_break = false;
	bool is_able_transfer_phase = true;
	bool is_land_vfx = false;
	

	CollisionDebugBox* collision_box_body;
	CollisionDebugBox* collision_box_back_leg;
	CollisionDebugBox* collision_box_front_leg;
	CollisionDebugBox* collision_box_center_leg;

	CollisionDebugBox* collision_box_attack;
	CollisionDebugBox* collision_box_spit_attack;
	CollisionDebugBox* collision_box_land_attack;
	CollisionDebugBox* collision_box_buff_attack;

	std::vector<CollisionDebugBox*> collision_boxes;
	std::vector<CollisionBoxInfo> collision_box_info{
		{},
		{},
		{},
		{},

		{},
		{},
		{},
		{}
	};

	bool is_tarnished_widow_take_light_damage_1_sfx = false;
	bool is_tarnished_widow_take_light_damage_2_sfx = false;
	bool is_tarnished_widow_take_heavy_damage_sfx   = false;
	bool is_tarnished_widow_walk_1_sfx              = false;
	bool is_tarnished_widow_walk_2_sfx              = false;
	bool is_tarnished_widow_jump_up_sfx             = false;
	bool is_tarnished_widow_land_sfx                = false;
	bool is_tarnished_widow_heavy_land_sfx          = false;
	bool is_tarnished_widow_attack_sfx              = false;
	bool is_tarnished_widow_spit_sfx                = false;
	bool is_tarnished_widow_buff_sfx                = false;
	bool is_tarnished_widow_roar_sfx                = false;
	bool is_tarnished_widow_death_sfx               = false;
};