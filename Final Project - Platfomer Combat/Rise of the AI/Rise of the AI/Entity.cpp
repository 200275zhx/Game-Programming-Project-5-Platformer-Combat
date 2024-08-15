#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "Entity.h"

// Default constructor
Entity::Entity()
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(0.0f), m_animation_cols(0), m_animation_index(0),
    m_animation_rows(0), m_animation_indices({}), m_animation_time(0.0f),
    m_texture_id(0), m_velocity(0.0f), m_acceleration(0.0f), m_width(0.0f), m_height(0.0f),
    m_direction(0), m_tex_width(1.0f), m_rotation_angle(0.0f)
{
    collision_box = new CollisionDebugBox(Utility::load_texture(COLLISION_DEBUG_BOX_FILEPATH));
    m_animation_sequences = { {} };
}

Entity::Entity(GLuint textureID, glm::vec3 position, glm::vec3 scale, std::vector<std::vector<int>> animation_sequences,
    int animation_cols, int animation_rows, float width, float height)
    : m_texture_id(textureID), m_position(position), m_scale(scale), m_movement(0.0f), m_model_matrix(1.0f),
    m_speed(0.0f), m_acceleration(0.0f), m_jumping_power(0.0f),
    m_animation_sequences(animation_sequences), m_animation_cols(animation_cols),
    m_animation_index(0), m_animation_rows(animation_rows), m_animation_indices({}),
    m_animation_time(0.0f), m_velocity(0.0f), m_width(width), m_height(height), 
    m_direction(0), m_tex_width(1.0f), m_rotation_angle(0.0f)
{
    collision_box = new CollisionDebugBox(Utility::load_texture(COLLISION_DEBUG_BOX_FILEPATH));
    m_animation_indices = m_animation_sequences[0];
}

// Parameterized constructor
Entity::Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power, 
    std::vector<std::vector<int>> animation_sequences, float animation_time,
    int animation_index, int animation_cols, int animation_rows, float width, float height
    )
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(speed), m_acceleration(acceleration), m_jumping_power(jump_power), 
    m_animation_sequences(animation_sequences), m_animation_cols(animation_cols),
    m_animation_index(animation_index), m_animation_rows(animation_rows), m_animation_indices({}),
    m_animation_time(animation_time), m_texture_id(texture_id), m_velocity(0.0f),
    m_width(width), m_height(height), m_direction(0), m_tex_width(1.0f), m_rotation_angle(0.0f)
{
    collision_box = new CollisionDebugBox(Utility::load_texture(COLLISION_DEBUG_BOX_FILEPATH));
    m_animation_indices = m_animation_sequences[0];
}

Entity::Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power,
    std::vector<std::vector<int>> animation_sequences, float animation_time,
    int animation_index, int animation_cols, int animation_rows, float width, float height,
    float tex_width_ratio)

    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(speed), m_acceleration(acceleration), m_jumping_power(jump_power),
    m_animation_sequences(animation_sequences), m_animation_cols(animation_cols),
    m_animation_index(animation_index), m_animation_rows(animation_rows), m_animation_indices({}),
    m_animation_time(animation_time), m_texture_id(texture_id), m_velocity(0.0f),
    m_width(width), m_height(height), m_direction(0), m_tex_width(tex_width_ratio), m_rotation_angle(0.0f)
{
    collision_box = new CollisionDebugBox(Utility::load_texture(COLLISION_DEBUG_BOX_FILEPATH));
    m_animation_indices = m_animation_sequences[0];
}

Entity::Entity(GLuint texture_id, float speed, float width, float height) 
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(speed), m_animation_cols(0), m_animation_index(0),
    m_animation_rows(0), m_animation_indices({}), m_animation_time(0.0f),
    m_texture_id(texture_id), m_velocity(0.0f), m_acceleration(0.0f), m_width(width), m_height(height), 
    m_direction(0), m_tex_width(1.0f), m_rotation_angle(0.0f)
{
    collision_box = new CollisionDebugBox(Utility::load_texture(COLLISION_DEBUG_BOX_FILEPATH));
    m_animation_sequences = { {} };
}

Entity::~Entity() { delete collision_box; }

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;

    // Step 2: Calculate its UV size
    float width = 1.0f / m_animation_cols / m_tex_width;
    float height = 1.0f / (float)m_animation_rows;

    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

bool const Entity::check_collision(Entity* other) const
{
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width * other->get_scale().x) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height * other->get_scale().y) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}

bool const Entity::check_collision(glm::vec3 probe, float width, float height) const
{
    float x_distance = fabs(m_position.x - probe.x) - ((m_width * m_scale.x + width) / 2.0f);
    float y_distance = fabs(m_position.y - probe.y) - ((m_height * m_scale.y + height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}

bool const Entity::check_collision(glm::vec3 probe_a, float width_a, float height_a, glm::vec3 probe_b, float width_b, float height_b) const {
    float x_distance = fabs(probe_a.x - probe_b.x) - ((width_a + width_b) / 2.0f);
    float y_distance = fabs(probe_a.y - probe_b.y) - ((height_a + height_b) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}

void const Entity::check_collision_y(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            float y_distance = fabs(m_position.y - collidable_entity->m_position.y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->m_height / 2.0f));
            if (m_velocity.y > 0)
            {
                m_position.y -= y_overlap;
                m_velocity.y = 0;

                // Collision!
                m_collided_top = true;
            }
            else if (m_velocity.y < 0)
            {
                m_position.y += y_overlap;
                m_velocity.y = 0;

                // Collision!
                m_collided_bottom = true;
            }
        }
    }
}

void const Entity::check_collision_x(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            float x_distance = fabs(m_position.x - collidable_entity->m_position.x);
            float x_overlap = fabs(x_distance - (m_width / 2.0f) - (collidable_entity->m_width / 2.0f));
            if (m_velocity.x > 0)
            {
                m_position.x -= x_overlap;
                m_velocity.x = 0;

                // Collision!
                m_collided_right = true;

            }
            else if (m_velocity.x < 0)
            {
                m_position.x += x_overlap;
                m_velocity.x = 0;

                // Collision!
                m_collided_left = true;
            }
        }
    }
}

void const Entity::check_collision_y(Map* map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    //glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    //glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2) - 0.01f, m_position.y + (m_height / 2), m_position.z);

    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    //glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    //glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2) - 0.01f, m_position.y - (m_height / 2), m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    //else if (map->is_solid(top_left, &penetration_x, &penetration_y) && m_velocity.y > 0)
    //{
    //    m_position.y -= penetration_y;
    //    m_velocity.y = 0;
    //    m_collided_top = true;
    //}
    //else if (map->is_solid(top_right, &penetration_x, &penetration_y) && m_velocity.y > 0)
    //{
    //    m_position.y -= penetration_y;
    //    m_velocity.y = 0;
    //    m_collided_top = true;
    //}

    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
    //else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && m_velocity.y < 0)
    //{
    //    m_position.y += penetration_y;
    //    m_velocity.y = 0;
    //    m_collided_bottom = true;
    //}
    //else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && m_velocity.y < 0)
    //{
    //    m_position.y += penetration_y;
    //    m_velocity.y = 0;
    //    m_collided_bottom = true;
    //}
}

void const Entity::check_collision_x(Map* map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
    }
}

void Entity::check_collision_objects(const std::vector<StaticAnimatedObject*>& objects) {
    for (StaticAnimatedObject* object : objects) {
        if (object->get_is_collidable()) {
            float x_distance = fabs(m_position.x - object->get_position().x) - ((m_width + object->get_width() * object->get_scale().x) / 2.0f);
            float y_distance = fabs(m_position.y - object->get_position().y) - ((m_height + object->get_height() * object->get_scale().y) / 2.0f);

            if (x_distance < 0.0f && y_distance < 0.0f)
            {
                // y collision check disable for unknown bug
                //float y_overlap = fabs(y_distance);
                //if (m_velocity.y > 0)
                //{
                //    m_position.y -= y_overlap;
                //    m_velocity.y = 0;
                //    // Collision!
                //    m_collided_top = true;
                //}
                //else if (m_velocity.y < 0)
                //{
                //    m_position.y += y_overlap;
                //    m_velocity.y = 0;
                //    // Collision!
                //    m_collided_bottom = true;
                //} 

                float x_overlap = fabs(x_distance);
                if (m_velocity.x > 0)
                {
                    m_position.x -= x_overlap;
                    m_velocity.x = 0;
                    // Collision!
                    m_collided_right = true;

                }
                else if (m_velocity.x < 0)
                {
                    m_position.x += x_overlap;
                    m_velocity.x = 0;
                    // Collision!
                    m_collided_left = true;
                }
            }
        }
    }
}

void Entity::update(float delta_time, Map* map)
{
    if (!m_is_active) return;
    //if (m_entity_type == ENEMY) ai_activate(player);

    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;

    // --------- UPDATE PHYSICS ---------- //

    m_velocity.x = m_movement.x * m_speed;
    m_velocity += m_acceleration * delta_time;

    if (m_is_jumping)
    {
        m_is_jumping = false;
        m_velocity.y += m_jumping_power;
    }

    check_collision_y(map);

    m_position.x += m_velocity.x * delta_time;
    check_collision_x(map);

    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position + m_texture_offset);
    m_model_matrix = glm::rotate(m_model_matrix, m_rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    m_model_matrix = glm::scale(m_model_matrix, m_scale);

    // --------- UPDATE ANIMATION ---------- //

    loop_play(delta_time);

    // --------- DEBUG COLLISION ---------- //

    if (m_is_check_collision) collision_box->update(m_position, m_scale, glm::vec3(m_width, m_height, 1.0f));
}

void Entity::loop_play(float delta_time) {
    m_animation_time += delta_time;
    float frames_per_second = (float)1 / SECONDS_PER_FRAME;

    if (m_animation_time >= frames_per_second)
    {
        m_animation_time = 0.0f;
        m_animation_index++;
        if (m_animation_index >= m_animation_indices.size()) { m_animation_index = 0; }
    }
}

void Entity::play_once(float delta_time) {
    is_current_animation_complete = false;
    if (m_animation_index == m_animation_indices.size() - 1) {
        is_current_animation_complete = true;
        return;
    }
    loop_play(delta_time);
}


void Entity::render(ShaderProgram* program)
{
    if (!m_is_active) return;
    render_debug(program);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    if (m_reflection_enabled) {
        glm::mat4 reflectionMatrix = m_model_matrix;

        reflectionMatrix = glm::translate(reflectionMatrix, glm::vec3(0.0f, -m_reflection_offset, 0.0f));
        reflectionMatrix = glm::scale(reflectionMatrix, glm::vec3(1.0f, -1.0f, 1.0f));

        program->set_model_matrix(reflectionMatrix);

        glUniform1i(glGetUniformLocation(program->get_program_id(), "isReflection"), 1);
        glUniform1f(glGetUniformLocation(program->get_program_id(), "reflectionAlpha"), m_reflection_alpha);

        if (!m_animation_indices.empty())
        {
            if (m_animation_index < m_animation_indices.size()) {
                draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
            }
            else {
                m_animation_index = 0;
                draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
            }
        }
        else {
            glBindTexture(GL_TEXTURE_2D, m_texture_id);

            glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(program->get_position_attribute());
            glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
            glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisableVertexAttribArray(program->get_position_attribute());
            glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
        }
    }

    program->set_model_matrix(m_model_matrix);
    glUniform1i(glGetUniformLocation(program->get_program_id(), "isReflection"), 0);  // Disable reflection logic

    if (!m_animation_indices.empty())
    {
        if (m_animation_index < m_animation_indices.size()) { 
            draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        }
        else {
            m_animation_index = 0;
            draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        }
        return;
    }

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}