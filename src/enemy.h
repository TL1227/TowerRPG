#ifndef ENEMY_H
#define ENEMY_H

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
    int DamageTexture;
    float PlayerDirection;
    float Alpha = 0.0f;
    Shader Shader;
    glm::mat4 ModelMat;
    float MaxHealth = 100;
    float HealthPoints = 100;

    Enemy(::Shader&);
    void Tick(float delta, glm::mat4);
    void SwitchToAttackTex();
    void SwitchToCalmTex();
    void SwitchToDamageTex();
private:
    void OnMoveActionChange(MoveAction) override;
    void OnDirectionChange(Cardinal) override;
    void OnMoveDistanceChange(float) override;

    void OnBattlePhaseChange(BattlePhase) override;
    void OnEnemyDamage(float dmgPercent) override;
    void Shake(float delta);
    void Fade(float delta);
    bool DrawMe = false;
    bool DamageMe = false;
    bool FadeOut = false;
    glm::vec3 BattlePosition;
};

#endif
