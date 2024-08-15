#include "Scene.h"

void Scene::switch_music(Mix_Music* current_bgm, const std::string& filepath, int volume) {
    Mix_HaltMusic();

    current_bgm = Mix_LoadMUS(filepath.c_str());
    if (!current_bgm) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }
    Mix_PlayMusic(current_bgm, -1);
    Mix_VolumeMusic(volume);
}