#pragma once

#include "shader.h"

class Drawable{
    public:
    virtual void Draw(Shader shader) = 0;

    virtual ~Drawable() {}
};