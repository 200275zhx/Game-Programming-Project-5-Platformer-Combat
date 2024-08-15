#pragma once
#include "Utility.h"
#include "CollisionDebugBox.h"

enum ObjectType {
    GATE,
    GATE_ACTUATOR,
    DECORATION
};

class StaticAnimatedObject {
public:
    static constexpr int SECONDS_PER_FRAME = 15;

	StaticAnimatedObject(GLuint textureID, glm::vec3 position, float radian, glm::vec3 rotate_direction, glm::vec3 scale, 
        std::vector<std::vector<int>> animation_sequences, int animation_cols, int animation_rows, float width, float height, 
        bool debug_collision, glm::vec3 texture_offset, float texture_width, bool is_collidable);
    ~StaticAnimatedObject();
	virtual void update(float delta_time) = 0;
	void render(ShaderProgram* program);

    const glm::vec3 get_position()          const { return m_position; }
    const glm::vec3 get_scale()             const { return m_scale; }
    const glm::vec3 get_texture_offset()    const { return m_texture_offset; }
    const float     get_width()             const { return m_width; }
    const float     get_height()            const { return m_height; }
    const bool      get_is_active()         const { return m_is_active; }
    const bool      get_is_collidable()     const { return m_is_collidable; }

    virtual const ObjectType    get_type()              const = 0;
    virtual const bool          get_is_interactive()    const = 0;

    void set_is_collidable(bool is_collidable) { m_is_collidable = is_collidable; }

protected:
    bool m_is_active;
    bool m_is_collidable;
	GLuint m_texture_id;
    glm::mat4 m_model_matrix;
    glm::vec3 m_position;
    glm::vec3 m_rotation_dir;
    glm::vec3 m_scale;
    glm::vec3 m_texture_offset;
    float m_rotation_angle;

    // Animation

    int m_animation_cols,
        m_animation_rows,
        m_animation_index,
        m_tex_width;

    bool is_current_animation_complete;
    std::vector<std::vector<int>> m_animation_sequences;
    std::vector<int> m_animation_indices;
    float m_animation_time;

    float   m_width,
            m_height;

    // Debug

    bool m_is_check_collision;
    CollisionDebugBox* m_collision_box;

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    void loop_play(float delta_time);
    void play_once(float delta_time);
    virtual void render_debug(ShaderProgram* program) const;
};