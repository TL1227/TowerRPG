#ifndef ENEMY_H
#define ENEMY_H

#include <vector>

#include <glm/glm.hpp>

struct EnemyVertex
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class Enemy
{
public:
    unsigned int VBO, EBO, VAO;
    std::vector<EnemyVertex>       vertices;
    std::vector<unsigned int> indices;
    glm::vec3 Position;
    int Texture;
    float PlayerDirection;

    Enemy();
    void Draw();
    void Draw(int tex);
};

#endif
