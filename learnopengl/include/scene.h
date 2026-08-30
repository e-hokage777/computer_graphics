#pragma once

#include <vector>
#include "drawable.h"
#include "shader.h"

class Scene{
public:
    std::vector<Drawable*> drawables= {};

    void add(Drawable* drawable){
        drawables.push_back(drawable);
    }


    void render(Shader shader){
        for (auto drawable : drawables){
            drawable->Draw(shader);
        }
    }
};