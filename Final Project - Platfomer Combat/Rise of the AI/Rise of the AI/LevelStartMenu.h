#pragma once
#include "Scene.h"

class LevelStartMenu : public Scene {
public:
    ~LevelStartMenu();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
    void reset() override;
    const   std::string     get_current_scene_name()    const { return "Level Start Menu"; }
};
