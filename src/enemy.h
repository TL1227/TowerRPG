#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <glm/glm.hpp>

#include "movementeventlistener.h"

struct EnemyVertex
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class Enemy : public MovementEventListener
{
public:
    unsigned int VBO, EBO, VAO;
    std::vector<EnemyVertex>       vertices;
    std::vector<unsigned int> indices;
    glm::vec3 Position;
    int CalmTexture;
    int AttackTexture;
    int ActiveTexture;
    float PlayerDirection;

    Enemy();
    void Draw();
    void SwitchToAttackTex();
    void SwitchToCalmTex();
private:
    void OnMoveActionChange() override;
};

#endif
