#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"
#include "Character.h"
#include "Enemy.h"
#include "StaticAnimatedObject.h"
#include <nlohmann/json.hpp>

constexpr bool DEBUG_COLLISION = false;

constexpr int   PLAY_ONCE = 0,
                NEXT_CHNL = -1,  // next available channel
                MUTE_VOL = 0,
                MILS_IN_SEC = 1000,
                ALL_SFX_CHN = -1;


/**
    Notice that the game's state is now part of the Scene class, not the main file.
*/
struct GameState
{
    // ————— GAME OBJECTS ————— //
    Map* map;
    Character* player;
    std::vector<Enemy*> enemies;
    std::vector<StaticAnimatedObject*> objects;
    
    // ————— AUDIO ————— //
    Mix_Music *bgm;

    Mix_Chunk *jump_sfx;
    Mix_Chunk* walk_1_sfx;
    Mix_Chunk* walk_2_sfx;
    Mix_Chunk* land_sfx;
    Mix_Chunk* idle_attack_1_sfx;
    Mix_Chunk* idle_attack_2_sfx;
    Mix_Chunk* idle_attack_3_sfx;
    Mix_Chunk* dash_attack_1_sfx;
    Mix_Chunk* dash_attack_2_sfx;
    Mix_Chunk* jump_attack_1_sfx;
    Mix_Chunk* jump_attack_2_sfx;
    Mix_Chunk* dash_1_sfx;
    Mix_Chunk* dash_2_sfx;
    Mix_Chunk* take_damage_sfx;

    Mix_Chunk* flower_enemy_take_damage = nullptr;
    Mix_Chunk* flower_enemy_death = nullptr;
    Mix_Chunk* flower_enemy_attack = nullptr;

    Mix_Chunk* ui_select_sfx = nullptr;

    Mix_Chunk* gate_open = nullptr;
    Mix_Chunk* gate_close = nullptr;
    Mix_Chunk* gate_actuator_activate = nullptr;
    Mix_Chunk* floor_break = nullptr;

    Mix_Chunk* tarnished_widow_take_light_damage_1_sfx = nullptr;
    Mix_Chunk* tarnished_widow_take_light_damage_2_sfx = nullptr;
    Mix_Chunk* tarnished_widow_take_heavy_damage_sfx = nullptr;
    Mix_Chunk* tarnished_widow_walk_1_sfx = nullptr;
    Mix_Chunk* tarnished_widow_walk_2_sfx = nullptr;
    Mix_Chunk* tarnished_widow_jump_up_sfx = nullptr;
    Mix_Chunk* tarnished_widow_land_sfx = nullptr;
    Mix_Chunk* tarnished_widow_heavy_land_sfx = nullptr;
    Mix_Chunk* tarnished_widow_attack_sfx = nullptr;
    Mix_Chunk* tarnished_widow_spit_sfx = nullptr;
    Mix_Chunk* tarnished_widow_buff_sfx = nullptr;
    Mix_Chunk* tarnished_widow_roar_sfx = nullptr;
    Mix_Chunk* tarnished_widow_death_sfx = nullptr;
    
    // ————— POINTERS TO OTHER SCENES ————— //
    int curr_scene_id;
    int next_scene_id;
    int prev_scene_id;
};

class Scene {
protected:
    GameState m_game_state;
    nlohmann::json level_info;
    std::string data_file_path;
    bool has_data = true;
    float right_view_edge;
    float player_life;
    
public:
    // ————— ATTRIBUTES ————— //
    int m_number_of_enemies = 0;
    
    // ————— METHODS ————— //
    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    virtual void reset() = 0;
    
    // ————— GETTERS ————— //
    GameState   const           get_state()                 const { return m_game_state; }
    int         const           get_number_of_enemies()     const { return m_game_state.enemies.size(); }
    const       int             get_id()                    const { return m_game_state.curr_scene_id; }
    const       nlohmann::json  get_level_info()            const { return level_info; }
    const       std::string     get_data_file_path()        const { return data_file_path; }
    const       bool            get_has_data()              const { return has_data; }
    const       float           get_right_view_edge()       const { return right_view_edge; }

    void set_prev_scene_id(int id) { m_game_state.prev_scene_id = id; }
    void set_player_life(float life) { player_life = life; }
    void switch_music(Mix_Music* current_bgm, const std::string& filepath, int volume);

    nlohmann::json load_level_data(const std::string& file_name) const {
        std::ifstream inputFile(file_name);
        if (!inputFile) {
            std::cerr << "Unable to open file: " << file_name << std::endl;
            exit(1);
        }

        nlohmann::json LevelAssets;
        inputFile >> LevelAssets;
        inputFile.close();

        return LevelAssets;
    }
    
    void write_level_data(const nlohmann::json& jsonObject, const std::string& file_name) {
        std::cout << "calling write_level_data()\n";
        std::ofstream outFile(file_name);
        if (outFile.is_open()) {
            outFile << jsonObject.dump(4); // Pretty-print with 4 spaces indentation
            std::cout << "write level data to json file complete!\n";
            outFile.close();
        }
        else {
            std::cerr << "Unable to write file: " << file_name << std::endl;
            exit(1);
        }
    }
};
