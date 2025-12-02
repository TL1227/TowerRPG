#ifndef QUAD_H
#define QUAD_H

#include "shader.h"

class Quad
{
public:
    Quad(const std::string& texturePath);
    void Draw();
    unsigned int VAO, VBO, EBO, Texture;
    float x, y, scalex, scaley;
private:
    Shader Shader;
};

#endif
