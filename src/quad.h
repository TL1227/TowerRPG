#ifndef QUAD_H
#define QUAD_H

#include "shader.h"

class Quad
{
public:
    Quad() = default;
    Quad(const std::string& texturePath, Shader shader);
    void Draw();
    unsigned int VAO, VBO, EBO, Texture;
    float x, y, scalex, scaley;
private:
    Shader Shader;
};

#endif
