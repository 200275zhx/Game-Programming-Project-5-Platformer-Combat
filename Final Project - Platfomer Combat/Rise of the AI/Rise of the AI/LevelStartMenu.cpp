#include "LevelStartMenu.h"

constexpr char  CHICKBOY_FILEPATH[] = "assets/Sprites/Chick-Boy/ChikBoyFullPack_10x14.png"; // 10 * 14 spritesheet
constexpr char  UI_SELECT_SFX_FILEPATH[] = "assets/Sound/SFX/UI SFX/ui save.wav";
GLuint font_texture_id;

LevelStartMenu::~LevelStartMenu() {
	delete    m_game_state.player;
}
void LevelStartMenu::reset() {
    delete    m_game_state.player;
}

void LevelStartMenu::initialise() {
    m_game_state.curr_scene_id = 3;
    m_game_state.next_scene_id = -1;
    has_data = false;
    //data_file_path = "level_C_data.json";
    //level_info = load_level_data(data_file_path);
    right_view_edge = 100.0f;

    // Code from main.cpp's initialise()
    //ChickBoy//
    GLuint player_texture_id = Utility::load_texture(CHICKBOY_FILEPATH);

    std::vector<std::vector<int>> chickboy_animation =
    {
        { 0, 1, 2, 3, 4, 5 },                                   // idle
        { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },             // idle extra
        { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 },             // run
        { 27, 28 },                                             // jump start
        { 29 },                                                 // jumping
        { 30, 31, 32, 33 },                                     // jump end
        //{ 27, 28, 29, 30, 31, 32, 33 },                         // jump
        { 34 },                                                 // fall start
        { 35 },                                                 // falling
        { 36, 37, 38 },                                         // landing
        //{ 34, 35, 36, 37, 38 },                                 // fall
        { 42, 43 },												// double jump start
        { 44, 45 },												// double jump roll
        { 46, 47 },												// double jump top
        { 48 },													// double jump end
        //{ 42, 43, 44, 45, 46, 47, 48 },                         // double jump
        { 52, 53, 54, 55, 56, 57 },                             // wall grab
        { 58, 59, 60, 61 },                                     // wall slide
        { 62, 63, 64, 65 },                                     // floor slide
        { 66, 67, 68, 69, 70, 71, 72 },                         // dash
        { 4, 73, 74, 75 },                                      // take damage
        { 76, 77, 78, 79, 80 },                                 // attack 1
        { 81, 82, 83, 84 },                                     // attack 2
        { 90, 91 },                                             // attack 3
        { 92, 93 },                                             // attack 3 landing
        //{ 87, 88, 89, 90, 91, 92, 93 },                         // attack 3
        { 94, 95, 96, 97 },                                     // ducking
        { 97, 98, 99, 100 },                                    // crouch idle
        { 101, 102, 103, 104, 105, 106, 107, 108, 109, 110 },    // crouch walk
        //{ 0, 1, 111, 112, 113, 114, 115, 116, 116 },            // death 1
        //{ 117, 118, 119, 120, 121, 122, 123, 124, 124 }         // death 2
        { 85, 86, 81 }                                          // attack 2 to idle
    };

    glm::vec3 gravity = glm::vec3(0.0f, -30.0f, 0.0f);

    m_game_state.player = new Character(
        player_texture_id,         // texture id
        5.0f,                      // speed
        gravity,                   // acceleration
        9.0f,                      // jumping power
        chickboy_animation,        // animation index sets
        0.0f,                      // animation time
        0,                         // current animation index
        10,                        // animation column amount
        14,                        // animation row amount
        0.5f,                      // width
        1.0f                       // height
    );

    m_game_state.player->set_position(glm::vec3(9.0f, 0.0f, 0.0f));
    //m_game_state.player->set_is_movable(false);

    font_texture_id = Utility::load_texture("assets/font1.png");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.ui_select_sfx = Mix_LoadWAV(UI_SELECT_SFX_FILEPATH);

    Mix_VolumeChunk(
        m_game_state.ui_select_sfx,
        MIX_MAX_VOLUME / 2
    );
}
void LevelStartMenu::update(float delta_time) {
    m_game_state.player->set_position(glm::vec3(9.0f, 0.0f, 0.0f));
    m_game_state.player->set_animation_indices(0);
    m_game_state.player->loop_play(delta_time);
}
void LevelStartMenu::render(ShaderProgram* program) {
    m_game_state.player->render(program);
}
