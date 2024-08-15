#include "LevelA.h"
#include "Utility.h"
#include "FlowerEnemy.h"
#include "Gate.h"
#include "GateActuator.h"

#define LEVEL_WIDTH 50
#define LEVEL_HEIGHT 32

constexpr char  FLOWER_ENEMY_FILEPATH[] = "assets/Sprites/Enemy/Flower Enemy/Flower Enemy Full Pack.png",
                CHICKBOY_FILEPATH[] = "assets/Sprites/Chick-Boy/ChikBoyFullPack_10x14.png", // 10 * 14 spritesheet
                TILESET_FILEPATH[] = "assets/Tileset/DARK Edition Tileset No background.png"; // 14 * 16 tileset
constexpr char  LEVEL_A_BGM_FILEPATH[] = "assets/Sound/Background/Level A/1-02 - City Ruins - Rays of Light.mp3";

unsigned int LEVELA_DATA[] =
{
//    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49
    142, 142, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 142, 142,           // 0
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 1
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 2
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 3
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 156, 156, 156, 156, 157, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 4
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 5
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 6
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 127, 128, 129, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 7
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 128, 128, 128, 128, 129, 0  , 210, 183, 211, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 8
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 127, 128, 129, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 9
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 210, 182, 183, 184, 211, 0  , 0  , 141, 142, 143, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 10
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 11
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 12
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 13
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 14
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 15
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 155, 156, 157, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 16
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 17
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 18
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 210, 211, 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 19
    142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 155, 156, 156, 156, 156, 156, 156, 156, 156, 157, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 20
    156, 157, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 21
    0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 127, 128, 128, 128, 128, 128, 128, 128, 128, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 22
    0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 23
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 129, 0  , 0  , 155, 156, 156, 156, 156, 156, 156, 156, 156, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142,           // 24
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 155, 156, 157, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 155, 156,           // 25
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,           // 26
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,           // 27
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,           // 27
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 29
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 30
    142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 31
};

LevelA::~LevelA()
{
    //write_level_data(level_info, "level_A_data.json"); // store level data
    for (Enemy* enemy : m_game_state.enemies) delete enemy;
    for (StaticAnimatedObject* object : m_game_state.objects) delete object;
    m_game_state.enemies.clear();
    m_game_state.objects.clear();
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::reset() {
    for (Enemy* enemy : m_game_state.enemies) delete enemy;
    for (StaticAnimatedObject* object : m_game_state.objects) delete object;
    m_game_state.enemies.clear();
    m_game_state.objects.clear();
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise()
{
    m_game_state.curr_scene_id = 0;
    m_game_state.next_scene_id = -1;
    data_file_path = "level_A_data.json";
    level_info = load_level_data(data_file_path);
    right_view_edge = 41.0f;
    
    GLuint map_texture_id = Utility::load_texture(TILESET_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 14, 16);
    
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

    if (m_game_state.prev_scene_id < 0) m_game_state.player->set_position(glm::vec3(5.0f, -2.0f, 0.0f));
    else if (m_game_state.prev_scene_id == 1) m_game_state.player->set_position(glm::vec3(48.0f, -27.0f, 0.0f));
    else if (m_game_state.prev_scene_id == 2) m_game_state.player->set_position(glm::vec3(1.0f, -23.0f, 0.0f));
    else if (m_game_state.prev_scene_id == 3) m_game_state.player->set_position(glm::vec3(5.0f, -2.0f, 0.0f));

    if (player_life) m_game_state.player->set_life(player_life);
    
    // ----------------- ENEMIES ----------------- //
    //m_game_state.enemies.push_back(new HellBot (Utility::load_texture(HELLBOT_FILEPATH)));
    //m_game_state.enemies.push_back(new BlindingSpider(Utility::load_texture(BLINDING_SPIDER_FILEPATH)));
    GLuint flower_enemy_texture_id = Utility::load_texture(FLOWER_ENEMY_FILEPATH);
    m_game_state.enemies.push_back(new FlowerEnemy(flower_enemy_texture_id));
    m_game_state.enemies.push_back(new FlowerEnemy(flower_enemy_texture_id));
    m_game_state.enemies.push_back(new FlowerEnemy(flower_enemy_texture_id));
    m_game_state.enemies.push_back(new FlowerEnemy(flower_enemy_texture_id));
    m_game_state.enemies.push_back(new FlowerEnemy(flower_enemy_texture_id, FLOWERENEMY_WALKER));

    m_game_state.enemies[0]->set_position(glm::vec3(30.0f, -20.0f, 0.0f));
    m_game_state.enemies[1]->set_position(glm::vec3(32.0f, -26.0f, 0.0f));
    m_game_state.enemies[2]->set_position(glm::vec3(36.0f, -26.0f, 0.0f));
    m_game_state.enemies[3]->set_position(glm::vec3(22.0f, -5.0f, 0.0f));
    m_game_state.enemies[4]->set_position(glm::vec3(34.0f, -5.0f, 0.0f));

    // ----------------- OBJECTS ----------------- //

    m_game_state.objects.push_back(new Gate(glm::vec3(0.0f, -22.5f, 0.0f),          // position
                                            0.0f,                                   // rotation angle
                                            glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                            glm::vec3(2.0f),                        // scale
                                            DEBUG_COLLISION,                        // debug check collision
                                            0));                                    // id

    m_game_state.objects.push_back(new Gate(glm::vec3(38.0f, -26.5f, 0.0f),         // position
                                            0.0f,                                   // rotation angle
                                            glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                            glm::vec3(2.0f),                        // scale
                                            DEBUG_COLLISION,                        // debug check collision
                                            1));                                    // id

    m_game_state.objects.push_back(new Gate(glm::vec3(49.0f, -26.5f, 0.0f),         // position
                                            3.1415926f,                             // rotation angle
                                            glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                            glm::vec3(2.0f),                        // scale
                                            DEBUG_COLLISION,                        // debug check collision
                                            2));                                    // id

    m_game_state.objects.push_back(new GateActuator(glm::vec3(41.0f, -27.0f, 0.0f),         // position
                                                    0.0f,                                   // rotation angle
                                                    glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                                    glm::vec3(1.0f),                        // scale
                                                    DEBUG_COLLISION,                        // debug check collision
                                                    (Gate*) m_game_state.objects[1],        // gate it control
                                                    1));                                    // id

    m_game_state.objects.push_back(new GateActuator(glm::vec3(16.0f, -7.0f, 0.0f),          // position
                                                    0.0f,                                   // rotation angle
                                                    glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                                    glm::vec3(1.0f),                        // scale
                                                    DEBUG_COLLISION,                        // debug check collision
                                                    (Gate*) m_game_state.objects[2],        // gate it control
                                                    2));                                    // id

    for (size_t index{}; index < level_info["LevelA"]["interactive_items"]["gate_actuator"].size() - 1; ++index) {
        static_cast<GateActuator*>(m_game_state.objects[index + 3])->set_current_state(level_info["LevelA"]["interactive_items"]["gate_actuator"][index + 1]["state"]);
    }
    for (size_t index{}; index < level_info["LevelA"]["interactive_items"]["gate"].size(); ++index) {
        static_cast<Gate*>(m_game_state.objects[index])->set_current_state(level_info["LevelA"]["interactive_items"]["gate"][index]["state"]);
    }

    // ------------ DEBUG COLLISION ------------ //

    m_game_state.player->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[0]->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[1]->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[2]->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[3]->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[4]->debug_mode_collision(DEBUG_COLLISION);
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS(LEVEL_A_BGM_FILEPATH);
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

    // Flower Enemy SFX
    m_game_state.flower_enemy_take_damage = Mix_LoadWAV(FLOWER_ENEMY_TAKE_DAMAGE_SFX_FILEPATH);
    m_game_state.flower_enemy_death = Mix_LoadWAV(FLOWER_ENEMY_DEATH_SFX_FILEPATH);
    m_game_state.flower_enemy_attack = Mix_LoadWAV(FLOWER_ENEMY_ATTACK_SFX_FILEPATH);

    Mix_VolumeChunk(
        m_game_state.flower_enemy_take_damage,
        MIX_MAX_VOLUME
    );
    Mix_VolumeChunk(
        m_game_state.flower_enemy_death,
        MIX_MAX_VOLUME / 2
    );
    Mix_VolumeChunk(
        m_game_state.flower_enemy_attack,
        MIX_MAX_VOLUME / 2
    );

    // Gate SFX
    m_game_state.gate_open = Mix_LoadWAV(GATE_OPEN_SFX);
    Mix_VolumeChunk(
        m_game_state.gate_open,
        MIX_MAX_VOLUME / 2
    );

    m_number_of_enemies = m_game_state.enemies.size();
}

void LevelA::update(float delta_time)
{
    if (m_game_state.player->get_is_interacting()) {
        StaticAnimatedObject* interacting_item = m_game_state.player->closest_interactive_item(m_game_state.objects, 1.0f);
        if (interacting_item) {
            std::pair<std::string, std::pair<int, int>> interaction = m_game_state.player->interact(interacting_item);
            if (interaction.first != "") {
                if (interaction.first == "gate_actuator") {
                    //std::cout << "editing level A data!\n";
                    level_info["LevelA"]["interactive_items"]["gate_actuator"][interaction.second.first]["state"] = interaction.second.second;
                    level_info["LevelA"]["interactive_items"]["gate"][interaction.second.first]["state"] = (interaction.second.second + 1);
                    Mix_PlayChannel(1, m_game_state.gate_open, 0);
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
        if (!m_game_state.enemies[i]->get_is_enemy_take_damage_played()) {
            //std::cout << "flower_enemy_take_damage sfx\n";
            Mix_PlayChannel(1, m_game_state.flower_enemy_take_damage, 0);
            m_game_state.enemies[i]->set_is_enemy_take_damage_played(true);
        }
        if (!m_game_state.enemies[i]->get_is_death_played()) {
            //std::cout << "flower_enemy_death sfx\n";
            Mix_PlayChannel(1, m_game_state.flower_enemy_death, 0);
            m_game_state.enemies[i]->set_is_death_played(true);
        }
        if (!m_game_state.enemies[i]->get_is_attack_played()) {
            //std::cout << "flower_enemy_attack sfx\n";
            Mix_PlayChannel(1, m_game_state.flower_enemy_attack, 0);
            m_game_state.enemies[i]->set_is_attack_played(true);
        }
    }

    for (StaticAnimatedObject* object : m_game_state.objects) {
        object->update(delta_time);
    }

    glm::vec3 player_position = m_game_state.player->get_position();

    if (player_position.x >= right_view_edge) {
        Utility::fadeOutMusicBasedOnPosition(player_position.x, 41.0f, 49.0f);
    }
    if (player_position.x <= 8.0f) {
        Utility::fadeOutMusicBasedOnPosition(player_position.x, 5.0f, 0.0f);
    }

    // --------- SWITCH SCENE --------- //

    if (player_position.x >= 49.0f) m_game_state.next_scene_id = 1;
    if (player_position.x <= 0.0f) m_game_state.next_scene_id = 2;
}

void LevelA::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < m_game_state.enemies.size(); i++) {
        m_game_state.enemies[i]->render(program);
    }
    for (int i = 0; i < m_game_state.objects.size(); i++) {
        m_game_state.objects[i]->render(program);
    }
}
