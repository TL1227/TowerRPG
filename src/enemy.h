#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <glm/glm.hpp>

#include "movementeventlistener.h"
#include "battleeventlistener.h"
#include "shader.h"

class Enemy : public MovementEventListener, public BattleEventListener
{
public:
    unsigned int VBO, EBO, VAO;
    glm::vec3 Position;
    int CalmTexture;
    int PreAttackTexture;
    int AttackTexture;
    int ActiveTexture;
    float PlayerDirection;
    float Alpha = 0.0f;
    Shader Shader;
    glm::mat4 ModelMat;

    Enemy(::Shader&);
    void Tick(glm::mat4);
    void SwitchToAttackTex();
    void SwitchToCalmTex();
private:
    void OnMoveActionChange(MoveAction) override;
    void OnDirectionChange(Cardinal) override;
    void OnMoveDistanceChange(float) override;

    void OnBattlePhaseChange(BattlePhase) override;
    bool DrawMe = false;
};

#endif
