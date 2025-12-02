#include "ui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

UI::UI()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
    float OffScreenDistance = (float)ScreenWidth * 0.5f;

    Shader battleMenuShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    battleMenuShader.use();
	battleMenuShader.setMat4("projection", projection);
    float BattleMenuQuadEndPosY = (float)ScreenHeight * 0.2f;
    BattleMenu = { "textures\\battlemenu.jpg", battleMenuShader};
    BattleMenu.x = (float)ScreenWidth / 2.0f;
    BattleMenu.y = BattleMenuQuadEndPosY - OffScreenDistance;
    BattleMenu.scalex = (float)ScreenWidth * 0.65f;
    BattleMenu.scaley = (float)ScreenHeight * 0.25f;

    Shader enemyHpShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    enemyHpShader.use();
	enemyHpShader.setMat4("projection", projection);
    float EnemyHpInnerQuadEndPosY = (float)ScreenHeight * 0.93f;
    EnemyHealthBar = { "textures\\enemyhealthinner.jpg", enemyHpShader };
    EnemyHealthBar.x = ScreenWidth / 2.0f;
    EnemyHealthBar.y = EnemyHpInnerQuadEndPosY + OffScreenDistance; //set it up for sliding on to screen
    EnemyHealthBar.scalex = (float)ScreenWidth * 0.65f;
    EnemyHealthBar.scaley = (float)ScreenHeight * 0.03f;
    EnemyHealthBarSlider.duration = audio.PreBattleBgmLength * 0.2;
	EnemyHealthBarSlider.start = enemyHpInnerQuad.y;
    EnemyHealthBarSlider.end = EnemyHpInnerQuadEndPosY - 10;
}