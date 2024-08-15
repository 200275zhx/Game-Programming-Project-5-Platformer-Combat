/**
* Author: [Hershey Zhang]
* Assignment: [2D soul-like platformer]
* Date due: 2024-08-15, 1:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "LevelStartMenu.h"
#include "Utility.h"
#include "Effects.h"
#include "TarnishedWidow.h"

enum AppStatus { RUNNING, TERMINATED };

// ––––– CONSTANTS ––––– //
constexpr int WINDOW_WIDTH = 1920,
WINDOW_HEIGHT = 1080;

constexpr float BG_RED = 0.2f,
BG_BLUE = 0.2f,
BG_GREEN = 0.2f,
BG_OPACITY = 1.0f;

constexpr int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

constexpr int NUMBER_OF_TEXTURES = 1;
constexpr GLint LEVEL_OF_DETAIL = 0;
constexpr GLint TEXTURE_BORDER = 0;

constexpr float PLATFORM_OFFSET = 5.0f;

// ––––– FILE PATH ––––– //

constexpr char  V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
                F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr char  FONT_SPRITE_FILEPATH[] = "assets/font1.png";
//constexpr char  SPRITESHEET_FILEPATH[] = "assets/george_0.png",
//                PLATFORM_FILEPATH[] = "assets/platformPack_tile027.png",
//                ENEMY_FILEPATH[] = "assets/soph.png",
//                TILESET_FILEPATH[] = "assets/Tileset/DARK Edition Tileset with background.png", // 14 * 16 tileset
//                CHICKBOY_FILEPATH[] = "assets/Sprites/Chick-Boy/ChikBoyFullPack_10x14.png"; // 10 * 14 spritesheet
//
//constexpr char  BGM_FILEPATH[] = "assets/crypto.mp3",
//                SFX_FILEPATH[] = "assets/bounce.wav";

// ––––– VARIABLES ––––– //
Scene* g_current_scene;
LevelA* g_levelA;
LevelB* g_levelB;
LevelC* g_levelC;
LevelStartMenu* g_level_start_menu;
SDL_Joystick* gJoystick = nullptr;

Scene* g_levels[4];

SDL_Window* g_display_window;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

bool is_walk_1_played = false;
bool is_walk_2_played = false;
bool is_jump_played = true;
bool is_land_played = false;
bool is_dash_played = false;
bool is_idle_attack_played = false;
bool is_dash_attack_played = false;
bool is_jump_attack_played = false;
bool is_take_damage_played = false;
bool is_ui_select_triggered = false;
bool is_check_control = false;

int idle_attack_sfx = 1;
int dash_attack_sfx = 1;
int jump_attack_sfx = 1;

AppStatus g_app_status = RUNNING;
GLuint g_text_texture_id;
Effects* g_effects;

void switch_to_scene(Scene* scene);
void initialise();
void process_input_keyboard();
void process_input_joystick();
void update();
void render();
void shutdown();

// ––––– GENERAL FUNCTIONS ––––– //
void switch_to_scene(Scene* scene)
{
    if (g_current_scene) {
        scene->set_prev_scene_id(g_current_scene->get_id());
        if (g_current_scene->get_has_data()) 
            g_current_scene->write_level_data(g_current_scene->get_level_info(), g_current_scene->get_data_file_path());
        scene->set_player_life(g_current_scene->get_state().player->get_life());
        g_current_scene->reset();
    } else { scene->set_prev_scene_id(-1); }
    //std::cout << "delete prev level complete\n";
    g_current_scene = scene;
    //if (g_current_scene) std::cout << "g_current_scene is valid scene\n";
    g_current_scene->initialise(); // DON'T FORGET THIS STEP!
    //std::cout << "g_current_scene->initialise(); complete\n";
}

void initialise()
{
    // ––––– GENERAL STUFF ––––– //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
    g_display_window = SDL_CreateWindow("GIVE ME ANOTHER WEEK!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif
    // ––––– VIDEO STUFF ––––– //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -10.0f, 10.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // Initialize the first available joystick
    if (SDL_NumJoysticks() > 0) {
        gJoystick = SDL_JoystickOpen(0);
        if (gJoystick == nullptr) {
            std::cerr << "Warning: Unable to open joystick! SDL Error: " << SDL_GetError() << std::endl;
        }
        else {
            std::cout << "Joystick connected: " << SDL_JoystickName(gJoystick) << std::endl;
        }
    }
    else {
        std::cerr << "No joysticks connected!" << std::endl;
    }

    // ––––– GENERAL STUFF ––––– //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_text_texture_id = Utility::load_texture(FONT_SPRITE_FILEPATH);

    // ––––– LEVELS ––––– //

    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_levelC = new LevelC();
    g_level_start_menu = new LevelStartMenu();

    g_levels[0] = g_levelA;
    g_levels[1] = g_levelB;
    g_levels[2] = g_levelC;
    g_levels[3] = g_level_start_menu;

    // Preload all levels
    for (Scene* level : g_levels) {
        level->initialise();
        level->reset();
    }

    // Start at level A
    switch_to_scene(g_levels[3]);

    g_effects = new Effects(g_projection_matrix, g_view_matrix);
}

void process_input_keyboard()
{
    Character* player = (Character*) g_current_scene->get_state().player;
    if (player->get_current_action() != WALL_SLIDE) player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_app_status = TERMINATED;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {

            case SDLK_SPACE:
                // Jump
                if (g_current_scene->get_id() == 3) {
                    if (g_current_scene->get_state().ui_select_sfx && !is_ui_select_triggered && !is_check_control) {
                        Mix_PlayChannel(-1, g_current_scene->get_state().ui_select_sfx, 0);
                        is_ui_select_triggered = true;
                    }
                }
                else {
                    if (player->movable()) {
                        player->jump();
                        if (player->get_is_jumpable()) is_jump_played = false;
                    }
                }
                break;
            case SDLK_j:
                player->attack();
                break;

            case SDLK_LSHIFT:
                if (player->movable() && player->get_is_dashable()) {
                    player->set_animation_index(0);
                    player->dash();
                }
                if (g_current_scene->get_id() == 3 && is_check_control) {
                    Mix_PlayChannel(-1, g_current_scene->get_state().ui_select_sfx, 0);
                    is_check_control = false;
                }
                break;

            case SDLK_w:
                player->input_interact();
                if (g_current_scene->get_id() == 3) {
                    Mix_PlayChannel(-1, g_current_scene->get_state().ui_select_sfx, 0);
                    is_check_control = true;
                }
                break;

            case SDLK_RETURN:
                if (g_current_scene->get_id() == 3) {
                    if (g_current_scene->get_state().ui_select_sfx && !is_ui_select_triggered) {
                        Mix_PlayChannel(3, g_current_scene->get_state().ui_select_sfx, 0);
                        is_ui_select_triggered = true;
                    }
                }
            default:
                break;
            }

        default:
            break;
        }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    CharacterAction current_action = player->get_current_action();

    if (key_state[SDL_SCANCODE_A] && player->movable()) {
        player->move_left();
    }
    else if (key_state[SDL_SCANCODE_D] && player->movable()) {
        player->move_right();
    }

    if (glm::length(player->get_movement()) > 1.0f)
        player->normalise_movement();
}

void process_input_joystick() {
    Character* player = (Character*)g_current_scene->get_state().player;
    if (player->get_current_action() != WALL_SLIDE) player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_app_status = TERMINATED;
            break;

        case SDL_JOYBUTTONDOWN:
            if (event.jbutton.which == SDL_JoystickInstanceID(gJoystick)) {
                switch (event.jbutton.button) {
                case 2:  // B Button
                    if (g_current_scene->get_id() == 3) {
                        if (g_current_scene->get_state().ui_select_sfx && !is_ui_select_triggered && !is_check_control) {
                            Mix_PlayChannel(-1, g_current_scene->get_state().ui_select_sfx, 0);
                            is_ui_select_triggered = true;
                        }
                    }
                    else {
                        if (player->movable()) {
                            player->jump();
                            if (player->get_is_jumpable()) is_jump_played = false;
                        }
                    }
                    break;

                case 5:  // RB Button
                    player->attack();
                    break;

                case 1:  // A Button
                    if (player->movable() && player->get_is_dashable()) {
                        player->set_animation_index(0);
                        player->dash();
                    }
                    if (g_current_scene->get_id() == 3 && is_check_control) {
                        Mix_PlayChannel(-1, g_current_scene->get_state().ui_select_sfx, 0);
                        is_check_control = false;
                    }
                    break;

                case 3:  // Y Button
                    player->input_interact();
                    if (g_current_scene->get_id() == 3) {
                        Mix_PlayChannel(-1, g_current_scene->get_state().ui_select_sfx, 0);
                        is_check_control = true;
                    }
                    break;

                default:
                    break;
                }
            }
            break;

        default:
            break;
        }
    }

    int xAxis = SDL_JoystickGetAxis(gJoystick, 0);

    if (xAxis < -16384 && player->movable()) {
        player->move_left();
    }
    else if (xAxis > 16384 && player->movable()) {
        player->move_right();
    }

    if (glm::length(player->get_movement()) > 1.0f)
        player->normalise_movement();
}


void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }

    Character* player = (Character*)g_current_scene->get_state().player;

    while (delta_time >= FIXED_TIMESTEP)
    {
        g_current_scene->update(FIXED_TIMESTEP);
        g_effects->update(FIXED_TIMESTEP);

        glm::vec3 player_position = player->get_position() + glm::vec3(0.0f, 1.0f, 0.0f);
        g_view_matrix = glm::mat4(1.0f);
        if (player_position.x <= 8.0f) {
            g_view_matrix = glm::translate(g_view_matrix,
                glm::vec3(-8.0f, -player_position.y, 0.0f));
        }
        else if (player_position.x >= g_current_scene->get_right_view_edge()) {
            g_view_matrix = glm::translate(g_view_matrix,
                glm::vec3(-g_current_scene->get_right_view_edge(), -player_position.y, 0.0f));
        }
        else if (g_current_scene->get_id() != 3) {
            g_view_matrix = glm::translate(g_view_matrix, -player_position);
        }

        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;

    //std::cout << "player life: " << g_current_scene->get_state().player->get_life() << std::endl;

    // --------- VFX --------- //

    if (g_current_scene->get_id() == 2) {
        if (static_cast<TarnishedWidow*>(g_current_scene->get_state().enemies[0])->get_is_land_vfx()) {
            std::cout << "shake effect\n";
            g_effects->start(SHAKE, 1.0f);
        }
    }
    g_view_matrix = glm::translate(g_view_matrix, g_effects->get_view_offset());

    // --------- SFX --------- //

    if (Mix_Playing(3) == 0 && is_ui_select_triggered) {
        switch_to_scene(g_levels[0]);
        is_ui_select_triggered = false;
    }

    CharacterAction player_action                   = player->get_current_action();
    CharacterAction player_prev_action              = player->get_prev_action();
    CharacterState  player_state                    = player->get_current_state();
    int             player_index                    = player->get_animation_index();

    switch (player_action)
    {
    case RUNING:
        switch (player_index)
        {
        case 0:
            is_walk_1_played = false;
            is_walk_2_played = false;
            break;
        case 3:
            if (!is_walk_1_played) {
                //std::cout << "move left index 3\n";
                Mix_PlayChannel(0, g_current_scene->get_state().walk_1_sfx, 0);
                is_walk_1_played = true;
            }
            break;
        case 4:
            is_walk_1_played = false;
            break;
        case 8:
            if (!is_walk_2_played) {
                //std::cout << "move left index 8\n";
                Mix_PlayChannel(0, g_current_scene->get_state().walk_2_sfx, 0);
                is_walk_2_played = true;
            }
            break;
        case 9:
            is_walk_2_played = false;
            break;
        default:
            break;
        }
        break;
    case JUMP_START:
    case DOUBLE_JUMP_START:
        if (!is_jump_played) {
            Mix_PlayChannel(0, g_current_scene->get_state().jump_sfx, 0);
            //std::cout << "jump sfx\n";
            is_jump_played = true;
        }
        break;
    case LANDING:
        if (!is_land_played) {
            Mix_PlayChannel(0, g_current_scene->get_state().land_sfx, 0);
            //std::cout << "land sfx\n";
            is_land_played = true;
        }
        break;
    case DASH:
        if (!is_dash_played) {
            //std::cout << "dash 1 sfx\n";
            Mix_PlayChannel(0, g_current_scene->get_state().dash_1_sfx, 0);
            is_dash_played = true;
        }
        break;
    case TAKE_DAMAGE:
        if (!is_take_damage_played) {
            //std::cout << "take damage sfx\n";
            Mix_PlayChannel(0, g_current_scene->get_state().take_damage_sfx, 0);
            is_take_damage_played = true;
        }
        break;
    case ATTACK_1:
        if (!is_idle_attack_played) {
            if (idle_attack_sfx == 2) {
                Mix_PlayChannel(0, g_current_scene->get_state().idle_attack_2_sfx, 0);
                //std::cout << "idle attack 2 sfx\n";
                ++idle_attack_sfx;
                is_idle_attack_played = true;
                break;
            }
            if (idle_attack_sfx == 1) {
                Mix_PlayChannel(0, g_current_scene->get_state().idle_attack_1_sfx, 0);
                //std::cout << "idle attack 1 sfx\n";
                ++idle_attack_sfx;
                is_idle_attack_played = true;
                break;
            }
            if (idle_attack_sfx == 3) {
                Mix_PlayChannel(0, g_current_scene->get_state().idle_attack_3_sfx, 0);
                //std::cout << "idle attack 3 sfx\n";
                idle_attack_sfx = 1;
                is_idle_attack_played = true;
                break;
            }
        }
        break;
    case ATTACK_2:
        if (!is_dash_attack_played) {
            if (dash_attack_sfx == 2) {
                Mix_PlayChannel(0, g_current_scene->get_state().dash_attack_2_sfx, 0);
                //std::cout << "dash attack 2 sfx\n";
                dash_attack_sfx = 1;
                is_dash_attack_played = true;
                break;
            }
            if (dash_attack_sfx == 1) {
                Mix_PlayChannel(0, g_current_scene->get_state().dash_attack_1_sfx, 0);
                //std::cout << "dash attack 1 sfx\n";
                ++dash_attack_sfx;
                is_dash_attack_played = true;
                break;
            }
        }
        break;
    case ATTACK_3:
        if (!is_jump_attack_played) {
            if (jump_attack_sfx == 2) {
                Mix_PlayChannel(0, g_current_scene->get_state().jump_attack_2_sfx, 0);
                //std::cout << "jump attack 2 sfx\n";
                jump_attack_sfx = 1;
                is_jump_attack_played = true;
                break;
            }
            if (jump_attack_sfx == 1) {
                Mix_PlayChannel(0, g_current_scene->get_state().jump_attack_1_sfx, 0);
                //std::cout << "jump attack 1 sfx\n";
                ++jump_attack_sfx;
                is_jump_attack_played = true;
                break;
            }
        }
        break;
    default:
        break;
    }
    
    switch (player_prev_action)
    {
    case LANDING:
        if (player_action != LANDING)
            is_land_played = false;
        break;
    case DASH:
        if (player_action != DASH)
            is_dash_played = false;
        break;
    case TAKE_DAMAGE:
        if (player_action != TAKE_DAMAGE)
            is_take_damage_played = false;
        break;
    case ATTACK_1:
        if (player_action != ATTACK_1)
            is_idle_attack_played = false;
        break;
    case ATTACK_2:
        if (player_action != ATTACK_2)
            is_dash_attack_played = false;
        break;
    case ATTACK_3:
        if (player_action != ATTACK_3)
            is_jump_attack_played = false;
        break;
    default:
        break;
    }
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);

    glClear(GL_COLOR_BUFFER_BIT);

    g_current_scene->render(&g_shader_program);

    Character* player = static_cast<Character*>(g_current_scene->get_state().player);
    std::vector<Enemy*> enemies = g_current_scene->get_state().enemies;
    if (!player->get_life()) {
        Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("YOU LOSE!"),
            0.25f, 0.0f, player->get_position() + glm::vec3(0.0f, 2.0f, 0.0f));
    }

    if (g_current_scene->get_id() == 3) {
        if (!is_check_control) {
            Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("GIVE ME ANOTHER WEEK"), 0.35f, 0.01f, glm::vec3(-3.4f, 1.5f, 0.0f));
            if (gJoystick) {
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("PRESS B TO START"), 0.25f, 0.01f, glm::vec3(-1.95f, -1.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("PRESS Y TO CHECK CONTROL"), 0.25f, 0.01f, glm::vec3(-3.0f, -2.0f, 0.0f));
            }
            else {
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("PRESS ENTER TO START"), 0.25f, 0.01f, glm::vec3(-2.4f, -1.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("PRESS W TO CHECK CONTROL"), 0.25f, 0.01f, glm::vec3(-3.0f, -2.0f, 0.0f));
            }
        }
        else {
            if (gJoystick) {
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("CONTROL MENU"), 0.35f, 0.01f, glm::vec3(-4.0f, 3.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("A                  : DASH          "), 0.25f, 0.01f, glm::vec3(-4.0f, 1.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("B                  : JUMP          "), 0.25f, 0.01f, glm::vec3(-4.0f, 1.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("Y                  : INTERACT      "), 0.25f, 0.01f, glm::vec3(-4.0f, 0.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("RB                 : ATTACK        "), 0.25f, 0.01f, glm::vec3(-4.0f, 0.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("A + RB             : DASH ATTACK   "), 0.25f, 0.01f, glm::vec3(-4.0f, -0.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("AIRBORNE + RB      : JUMP ATTACK   "), 0.25f, 0.01f, glm::vec3(-4.0f, -1.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("AIRBORNE + B       : DOUBLE JUMP   "), 0.25f, 0.01f, glm::vec3(-4.0f, -1.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("WALL SLIDE + B     : WALL JUMP     "), 0.25f, 0.01f, glm::vec3(-4.0f, -2.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("PRESS A TO RETURN"), 0.25f, 0.01f, glm::vec3(-4.0f, -3.0f, 0.0f));
            }
            else {
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("CONTROL MENU"), 0.35f, 0.01f, glm::vec3(-4.0f, 3.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("LEFT SHIFT         : DASH          "), 0.25f, 0.01f, glm::vec3(-4.0f, 1.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("SPACE              : JUMP          "), 0.25f, 0.01f, glm::vec3(-4.0f, 1.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("W                  : INTERACT      "), 0.25f, 0.01f, glm::vec3(-4.0f, 0.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("J                  : ATTACK        "), 0.25f, 0.01f, glm::vec3(-4.0f, 0.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("LEFT SHIFT + J     : DASH ATTACK   "), 0.25f, 0.01f, glm::vec3(-4.0f, -0.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("AIRBORNE + J       : JUMP ATTACK   "), 0.25f, 0.01f, glm::vec3(-4.0f, -1.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("AIRBORNE + SPACE   : DOUBLE JUMP   "), 0.25f, 0.01f, glm::vec3(-4.0f, -1.5f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("WALL SLIDE + SPACE : WALL JUMP     "), 0.25f, 0.01f, glm::vec3(-4.0f, -2.0f, 0.0f));
                Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("PRESS LEFT SHIFT TO RETURN"), 0.25f, 0.01f, glm::vec3(-4.0f, -3.0f, 0.0f));
            }
        }
    }

    g_effects->render();

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    delete g_levelA;
    delete g_levelB;
    delete g_levelC;
    delete g_level_start_menu;
    delete g_effects;
}

// ––––– GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();

    while (g_app_status == RUNNING)
    {
        if (gJoystick) process_input_joystick();
        else process_input_keyboard();

        update();
        if (g_current_scene->get_state().next_scene_id >= 0) {
            std::cout << "\nswitching to scene: " << g_current_scene->get_state().next_scene_id << std::endl;
            switch_to_scene(g_levels[g_current_scene->get_state().next_scene_id]);
        }
        render();
    }

    shutdown();
    return 0;
}