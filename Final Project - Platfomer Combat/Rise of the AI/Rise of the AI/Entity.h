#ifndef ENTITY_H
#define ENTITY_H

#include "glm/glm.hpp"
#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <cmath>
#include "Map.h"
#include "CollisionDebugBox.h"
#include "StaticAnimatedObject.h"

const glm::vec3 GRAVITY = glm::vec3(0.0f, -30.0f, 0.0f);
constexpr char COLLISION_DEBUG_BOX_FILEPATH[] = "assets/Sprites/collision_area.png";

class Entity
{
protected:
    bool m_is_active = true;
    bool m_is_check_collision = false;
    CollisionDebugBox* collision_box = nullptr;
    glm::vec3 m_texture_offset = glm::vec3(0.0f);

    // ————— TRANSFORMATIONS ————— //
    glm::vec3 m_movement;
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    glm::mat4 m_model_matrix;

    float   m_speed,
            m_jumping_power,
            m_rotation_angle;

    bool m_is_jumping;
    bool m_is_attacking;

    // ————— TEXTURES ————— //
    GLuint    m_texture_id;
    bool m_reflection_enabled = false;
    float m_reflection_alpha = 0.5;
    float m_reflection_offset = 1.0f;

    // ————— ANIMATION ————— //
    int m_animation_cols,
        m_animation_rows,
        m_animation_index,
        m_tex_width;

    bool is_current_animation_complete = true;
    std::vector<std::vector<int>> m_animation_sequences;
    std::vector<int> m_animation_indices;
    float m_animation_time = 0.0f;
    int m_direction;

    float m_width = 1.0f,
          m_height = 1.0f;
    // ————— COLLISIONS ————— //
    bool m_collided_top = false;
    bool m_collided_bottom = false;
    bool m_collided_left = false;
    bool m_collided_right = false;

public:
    // ————— STATIC VARIABLES ————— //
    static constexpr int SECONDS_PER_FRAME = 15;

    // ————— METHODS ————— //
    Entity();
    Entity(GLuint textureID, glm::vec3 position, glm::vec3 scale, std::vector<std::vector<int>> animation_sequences, 
        int animation_cols, int animation_rows, float width, float height); // Animated Static Object Constructor
    Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power, 
        std::vector<std::vector<int>> animation_sequences, float animation_time,
        int animation_index, int animation_cols, int animation_rows, float width, float height);

    Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power,
        std::vector<std::vector<int>> animation_sequences, float animation_time,
        int animation_index, int animation_cols, int animation_rows, float width, float height,
        float tex_width_ratio); // AI constructor

    Entity(GLuint texture_id, float speed, float width, float height); // AI constructor
    ~Entity();

    void loop_play(float delta_time);
    void play_once(float delta_time);

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    bool const check_collision(Entity* other) const;
    bool const check_collision(glm::vec3 probe, float width, float height) const;
    bool const check_collision(glm::vec3 probe_a, float width_a, float height_a, glm::vec3 probe_b, float width_b, float height_b) const;
    void const check_collision_y(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_y(Map* map);
    void const check_collision_x(Map* map);
    void check_collision_objects(const std::vector<StaticAnimatedObject*>& objects);

    virtual void update(float delta_time, Map* map);
    virtual void render(ShaderProgram* program);
    virtual void render_debug(ShaderProgram* program) const { if (m_is_check_collision) collision_box->render(program); }

    void normalise_movement() { m_movement = glm::normalize(m_movement); }
    virtual void move_left() { m_movement.x = -1.0f; }
    virtual void move_right() { m_movement.x = 1.0f; }
    virtual void const jump() { m_is_jumping = true; }
    virtual void attack() { m_is_attacking = true; }

    // ————— GETTERS ————— //
    glm::vec3 const get_position()     const { return m_position; }
    glm::vec3 const get_velocity()     const { return m_velocity; }
    glm::vec3 const get_acceleration() const { return m_acceleration; }
    glm::vec3 const get_movement()     const { return m_movement; }
    glm::vec3 const get_scale()        const { return m_scale; }
    GLuint    const get_texture_id()   const { return m_texture_id; }
    float     const get_speed()        const { return m_speed; }
    bool      const get_collided_top() const { return m_collided_top; }
    bool      const get_collided_bottom() const { return m_collided_bottom; }
    bool      const get_collided_right() const { return m_collided_right; }
    bool      const get_collided_left() const { return m_collided_left; }
    glm::mat4 const get_model_matrix() const { return m_model_matrix; }
    bool            get_is_active() const { return m_is_active; }
    const int       get_direction() const { return m_direction; }
    const int       get_animation_index() const { return m_animation_index; }
    const bool      get_is_current_animation_complete() const { return is_current_animation_complete; }

    void activate() { m_is_active = true; };
    void deactivate() { m_is_active = false; };
    // ————— SETTERS ————— //
    void const set_position(glm::vec3 new_position) { m_position = new_position; }
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; }
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration; }
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; }
    void const set_scale(glm::vec3 new_scale) { m_scale = new_scale; }
    void const set_texture_id(GLuint new_texture_id) { m_texture_id = new_texture_id; }
    void const set_speed(float new_speed) { m_speed = new_speed; }
    void const set_animation_cols(int new_cols) { m_animation_cols = new_cols; }
    void const set_animation_rows(int new_rows) { m_animation_rows = new_rows; }
    void const set_animation_index(int new_index) { m_animation_index = new_index; }
    void const set_animation_time(float new_time) { m_animation_time = new_time; }
    void const set_jumping_power(float new_jumping_power) { m_jumping_power = new_jumping_power; }
    void const set_width(float new_width) { m_width = new_width; }
    void const set_height(float new_height) { m_height = new_height; }
    void set_texture_offset(glm::vec3 texture_offset) { m_texture_offset = texture_offset; }
    void set_position_offset(glm::vec3 position_offset) { m_position += position_offset; }
    void debug_mode_collision(bool is_check_collision) { m_is_check_collision = is_check_collision; }
    void set_rotation(float radian) { m_rotation_angle = radian; }
    void set_animation_indices(int index) { m_animation_indices = m_animation_sequences[index]; }
};

#endif // ENTITY_H