#include "LevelB.h"
#include "Utility.h"
#include "HellBot.h"
#include "Gate.h"
#include "GateActuator.h"
#include "StaticDecoration.h"

#define LEVEL_WIDTH 29
#define LEVEL_HEIGHT 14

constexpr char  CHICKBOY_FILEPATH[] = "assets/Sprites/Chick-Boy/ChikBoyFullPack_10x14.png", // 10 * 14 spritesheet
                TILESET_FILEPATH[] = "assets/Tileset/DARK Edition Tileset No background.png", // 14 * 16 tileset    
                HELLBOT_FILEPATH[] = "assets/Sprites/Enemy/Hell Bot DARK/idle 92x36.png",
                ACTUATOR_BROKEN_1_FILEPATH[] = "assets/Sprites/Animated-Objects/Save/broken1.png",
                ACTUATOR_BROKEN_2_FILEPATH[] = "assets/Sprites/Animated-Objects/Save/broken2.png";

constexpr char  LEVEL_B_BGM_FILEPATH[] = "assets/Sound/Background/Level B/1-01 - Significance - Nothing.mp3",
                LEVEL_B_BATTLE_BGM_FILEPATH[] = "assets/Sound/Background/Level B/1-08 - A Beautiful Song - Nothing.mp3";

unsigned int LEVELB_DATA[] =
{
//    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,  // 0
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,  // 1
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,  // 2
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 156, 156, 156, 156, 156, 156, 156, 156, 156, 142, 142,  // 3
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 43 , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,  // 4
    142, 142, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 43 , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 155, 156,  // 5
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,  // 6
    156, 157, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,  // 7
    0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,  // 8
    0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,  // 9
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,  // 10
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,  // 11
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,  // 12
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142   // 13
};

LevelB::~LevelB()
{
    for (Enemy* enemy : m_game_state.enemies) delete enemy;
    for (StaticAnimatedObject* object : m_game_state.objects) delete object;
    m_game_state.enemies.clear();
    m_game_state.objects.clear();
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::reset() {
    for (Enemy* enemy : m_game_state.enemies) delete enemy;
    for (StaticAnimatedObject* object : m_game_state.objects) delete object;
    m_game_state.enemies.clear();
    m_game_state.objects.clear();
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise()
{
    m_game_state.curr_scene_id = 1;
    m_game_state.next_scene_id = -1;
    data_file_path = "level_B_data.json";
    level_info = load_level_data(data_file_path);
    right_view_edge = 20.0f;
    
    GLuint map_texture_id = Utility::load_texture(TILESET_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 14, 16);
    
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

    m_game_state.player->set_position(glm::vec3(1.0f, -9.0f, 0.0f));

    if (player_life) m_game_state.player->set_life(player_life);
    
    // ----------------- ENEMIES ----------------- //

    GLuint HellBot_texture_id = Utility::load_texture(HELLBOT_FILEPATH);
    m_game_state.enemies.push_back(new HellBot (HellBot_texture_id));
    m_game_state.enemies.push_back(new HellBot (HellBot_texture_id));
    m_game_state.enemies.push_back(new HellBot (HellBot_texture_id));

    m_game_state.enemies[0]->set_position(glm::vec3(6.0f, -9.0f, 0.0f));
    m_game_state.enemies[1]->set_position(glm::vec3(11.0f, -9.0f, 0.0f));
    m_game_state.enemies[2]->set_position(glm::vec3(12.0f, -9.0f, 0.0f));

    // ----------------- OBJECTS ----------------- //
    m_game_state.objects.push_back(new Gate(glm::vec3(0.0f, -8.5f, 0.0f),           // position
                                            0.0f,                                   // rotation angle
                                            glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                            glm::vec3(2.0f),                        // scale
                                            DEBUG_COLLISION,                        // debug check collision
                                            0));                                    // id

    m_game_state.objects.push_back(new Gate(glm::vec3(28.0f, -7.5f, 0.0f),          // position
                                            3.1415926f,                             // rotation angle
                                            glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                            glm::vec3(4.0f),                        // scale
                                            DEBUG_COLLISION,                        // debug check collision
                                            1));                                    // id

    m_game_state.objects.push_back(new GateActuator(glm::vec3(22.0f, -9.0f, 0.0f),          // position
                                                    0.0f,                                   // rotation angle
                                                    glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                                    glm::vec3(1.0f),                        // scale
                                                    DEBUG_COLLISION,                        // debug check collision
                                                    nullptr,                                // gate it control
                                                    0));                                    // id

    m_game_state.objects.push_back(new StaticDecoration(Utility::load_texture(ACTUATOR_BROKEN_1_FILEPATH),      // texture id
                                                        glm::vec3(20.0f, -9.0f, 0.0f),                          // position
                                                        0.0f,                                                   // rotation angle
                                                        glm::vec3(0.0f, 1.0f, 0.0f),                            // rotation direction
                                                        glm::vec3(1.0f),                                        // scale
                                                        DEBUG_COLLISION));                                      // debug check collision

    m_game_state.objects.push_back(new StaticDecoration(Utility::load_texture(ACTUATOR_BROKEN_2_FILEPATH),      // texture id
                                                        glm::vec3(24.0f, -9.0f, 0.0f),                          // position
                                                        0.0f,                                                   // rotation angle
                                                        glm::vec3(0.0f, 1.0f, 0.0f),                            // rotation direction
                                                        glm::vec3(1.0f),                                        // scale
                                                        DEBUG_COLLISION));                                      // debug check collision

    static_cast<Gate*>(m_game_state.objects[0])->set_current_state(GATE_OPEN_IDLE);
    static_cast<GateActuator*>(m_game_state.objects[2])->set_current_state(level_info["LevelB"]["interactive_items"]["gate_actuator"][0]["state"]);
    

    // ------------ DEBUG COLLISION ------------ //

    m_game_state.player->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[0]->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[1]->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[2]->debug_mode_collision(DEBUG_COLLISION);

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS(LEVEL_B_BGM_FILEPATH);
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 10);

    // Character SFX
    m_game_state.jump_sfx = Mix_LoadWAV(JUMP_SFX_FILEPATH);
    m_game_state.land_sfx = Mix_LoadWAV(LAND_SFX_FILEPATH);
    m_game_state.walk_1_sfx = Mix_LoadWAV(WALK_1_SFX_FILEPATH);
    m_game_state.walk_2_sfx = Mix_LoadWAV(WALK_2_SFX_FILEPATH);
    m_game_state.dash_1_sfx = Mix_LoadWAV(DASH_1_SFX_FILEPATH);
    //m_game_state.dash_2_sfx = Mix_LoadWAV(DASH_2_SFX_FILEPATH);
    m_game_state.idle_attack_1_sfx = Mix_LoadWAV(IDLE_ATTACK_1_SFX_FILEPATH);
    m_game_state.idle_attack_2_sfx = Mix_LoadWAV(IDLE_ATTACK_2_SFX_FILEPATH);
    m_game_state.idle_attack_3_sfx = Mix_LoadWAV(IDLE_ATTACK_3_SFX_FILEPATH);
    m_game_state.dash_attack_1_sfx = Mix_LoadWAV(DASH_ATTACK_1_SFX_FILEPATH);
    m_game_state.dash_attack_2_sfx = Mix_LoadWAV(DASH_ATTACK_2_SFX_FILEPATH);
    m_game_state.jump_attack_1_sfx = Mix_LoadWAV(JUMP_ATTACK_1_SFX_FILEPATH);
    m_game_state.jump_attack_2_sfx = Mix_LoadWAV(JUMP_ATTACK_2_SFX_FILEPATH);
    m_game_state.take_damage_sfx = Mix_LoadWAV(TAKE_DAMAGE_SFX_FILEPATH);

    Mix_VolumeChunk(
        m_game_state.jump_sfx,
        MIX_MAX_VOLUME / 15
    );
    Mix_VolumeChunk(
        m_game_state.land_sfx,
        MIX_MAX_VOLUME / 8
    );

    Mix_VolumeChunk(
        m_game_state.walk_1_sfx,
        MIX_MAX_VOLUME
    );
    Mix_VolumeChunk(
        m_game_state.walk_2_sfx,
        MIX_MAX_VOLUME / 2
    );

    Mix_VolumeChunk(
        m_game_state.dash_1_sfx,
        MIX_MAX_VOLUME / 10
    );
    //Mix_VolumeChunk(
    //    m_game_state.dash_2_sfx,
    //    MIX_MAX_VOLUME / 10
    //);

    Mix_VolumeChunk(
        m_game_state.idle_attack_1_sfx,
        MIX_MAX_VOLUME / 20
    );
    Mix_VolumeChunk(
        m_game_state.idle_attack_2_sfx,
        MIX_MAX_VOLUME / 20
    );
    Mix_VolumeChunk(
        m_game_state.idle_attack_3_sfx,
        MIX_MAX_VOLUME / 20
    );

    Mix_VolumeChunk(
        m_game_state.dash_attack_1_sfx,
        MIX_MAX_VOLUME / 20
    );
    Mix_VolumeChunk(
        m_game_state.dash_attack_2_sfx,
        MIX_MAX_VOLUME / 20
    );
    Mix_VolumeChunk(
        m_game_state.jump_attack_1_sfx,
        MIX_MAX_VOLUME / 20
    );
    Mix_VolumeChunk(
        m_game_state.jump_attack_2_sfx,
        MIX_MAX_VOLUME / 20
    );

    Mix_VolumeChunk(
        m_game_state.take_damage_sfx,
        MIX_MAX_VOLUME / 10
    );
}

void LevelB::update(float delta_time)
{
    if (m_game_state.player->get_is_interacting()) {
        StaticAnimatedObject* interacting_item = m_game_state.player->closest_interactive_item(m_game_state.objects, 1.0f);
        if (interacting_item) {
            std::pair<std::string, std::pair<int, int>> interaction = m_game_state.player->interact(interacting_item);
            if (interaction.first != "") {
                if (interaction.first == "gate_actuator") {
                    //std::cout << "editing level B data!\n";
                    level_info["LevelB"]["interactive_items"]["gate_actuator"][interaction.second.first]["state"] = interaction.second.second;
                    nlohmann::json level_a_data = load_level_data("level_A_data.json");
                    level_a_data["LevelA"]["interactive_items"]["gate"][0]["state"] = 2;
                    write_level_data(level_a_data, "level_A_data.json");
                }
            }
        }
        m_game_state.player->set_is_interacting(false);
    }
    m_game_state.player->update(delta_time, m_game_state.enemies, m_game_state.objects, m_game_state.map);
    
    for (int i = 0; i < m_game_state.enemies.size(); i++) {
        m_game_state.enemies[i]->update(delta_time,
            m_game_state.player,
            m_game_state.map);
    }

    for (StaticAnimatedObject* object : m_game_state.objects) {
        object->update(delta_time);
    }

    glm::vec3 player_position = m_game_state.player->get_position();

    if (player_position.x <= 0.0f) m_game_state.next_scene_id = 0;

    if (player_position.x <= 8.0f) {
        Utility::fadeOutMusicBasedOnPosition(player_position.x, 5.0f, 0.0f);
    }
}

void LevelB::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < m_game_state.enemies.size(); i++) {
        m_game_state.enemies[i]->render(program);
    }
    //std::cout << "before render\n";
    for (int i = 0; i < m_game_state.objects.size(); i++) {
        m_game_state.objects[i]->render(program);
    }
    //std::cout << "after render\n";
}
