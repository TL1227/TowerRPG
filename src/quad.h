#ifndef QUAD_H
#define QUAD_H

#include "shader.h"

class Quad
{
public:
    Quad();
    Quad(const std::string& texturePath, Shader shader);
    void Draw();
    unsigned int VAO, VBO, EBO, Texture;
    float x, y, scalex, scaley;
    float Top();
    float Right();
    float Bottom();
    float Left();
private:
    Shader Shader;
};

#endif
