#pragma once
#include "StaticAnimatedObject.h"

StaticAnimatedObject::StaticAnimatedObject(GLuint textureID, glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale,
    std::vector<std::vector<int>> animation_sequences, int animation_cols, int animation_rows, float width, float height,
    bool debug_collision, glm::vec3 texture_offset, float texture_width, bool is_collidable)

	: m_texture_id(textureID), m_model_matrix(glm::mat4(1.0f)), m_animation_sequences(animation_sequences), m_animation_cols(animation_cols),
    m_animation_rows(animation_rows), m_animation_index(0), m_animation_indices({}), m_animation_time(0.0f), m_width(width),
	m_height(height), m_tex_width(texture_width), m_collision_box(nullptr), m_is_check_collision(debug_collision),
	is_current_animation_complete(true), m_is_active(true), m_position(position), m_rotation_angle(radian), m_rotation_dir(rotate_direction),
    m_scale(scale), m_texture_offset(texture_offset), m_is_collidable(is_collidable)
{
    m_collision_box = new CollisionDebugBox(Utility::load_texture("assets/Sprites/collision_area.png"));
    m_animation_indices = m_animation_sequences[0];

    m_model_matrix = glm::translate(m_model_matrix, position + texture_offset);
    m_model_matrix = glm::rotate(m_model_matrix, radian, rotate_direction);
    m_model_matrix = glm::scale(m_model_matrix, scale);

    if (m_is_check_collision) m_collision_box->update(position, scale, glm::vec3(m_width, m_height, 1.0f));
}

StaticAnimatedObject::~StaticAnimatedObject() { delete m_collision_box; }

void StaticAnimatedObject::render_debug(ShaderProgram* program) const { if (m_is_check_collision) m_collision_box->render(program); }

void StaticAnimatedObject::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
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

void StaticAnimatedObject::loop_play(float delta_time) {
    m_animation_time += delta_time;
    float frames_per_second = (float)1 / SECONDS_PER_FRAME;

    if (m_animation_time >= frames_per_second)
    {
        m_animation_time = 0.0f;
        m_animation_index++;
        if (m_animation_index >= m_animation_indices.size()) { m_animation_index = 0; }
    }
}

void StaticAnimatedObject::play_once(float delta_time) {
    is_current_animation_complete = false;
    if (m_animation_index == m_animation_indices.size() - 1) {
        is_current_animation_complete = true;
        return;
    }
    loop_play(delta_time);
}

void StaticAnimatedObject::render(ShaderProgram* program) {
	if (!m_is_active) return;
	render_debug(program);
    //std::cout << m_model_matrix[0][0] << std::endl;
	program->set_model_matrix(m_model_matrix);

    if (!m_animation_indices.empty()) {
        if (m_animation_index < m_animation_indices.size()) { draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]); }
        else {
            m_animation_index = 0;
            draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        }
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}