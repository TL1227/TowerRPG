#include "enemy.h"

#include "textures.h"
#include "cardinal.h"
#include <glm/gtc/matrix_transform.hpp>

Enemy::Enemy(::Shader& shader)
	: Shader{ shader }
{
    float vertices[] = {
        // positions          // colors           // texture coords
         0.2f,  0.4f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.2f, -0.4f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.2f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.2f,  0.4f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //AttackTexture= LoadTexture("textures\\goblinattack.jpg");
    //CalmTexture = LoadTexture("textures\\goblincalm.jpg");
    CalmTexture = LoadTexture("textures\\goblincalm.png");
    AttackTexture= LoadTexture("textures\\goblinattack.png");
    ActiveTexture = CalmTexture;

    glBindVertexArray(0);

    //TODO: set the starting direction some other wa!
    PlayerDirection = (float)Cardinal::East;
}

void Enemy::Tick(float delta, glm::mat4 view)
{
    if (DamageMe)
    {
        Shake(delta);
    }
    if (DrawMe)
    {
		Shader.use();
		ModelMat = glm::translate(glm::mat4(1.0f), Position);
		ModelMat = glm::rotate(ModelMat, glm::radians(PlayerDirection - 90), glm::vec3(0.0f, 1.0f, 0.0f)); //TODO: the rotation will flip depending on player direction. Fix this.
		ModelMat = glm::scale(ModelMat, glm::vec3(0.8f, 0.8f, 0.0f));
		Shader.setMat4("model", ModelMat);
		Shader.setMat4("view", view);
		Shader.setFloat("alpha", Alpha); //TODO: animate this fade without using CharMove.DistanceMoved

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, ActiveTexture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
    }
    if(FadeOut)
    {
        Fade(delta);
    }
}

void Enemy::OnMoveActionChange(MoveAction m)
{
    
}

void Enemy::OnDirectionChange(Cardinal c)
{
    PlayerDirection = (float)c;
}

//TODO: if these never get called anywhere else just inline it
void Enemy::SwitchToAttackTex() { ActiveTexture = AttackTexture; }
void Enemy::SwitchToCalmTex() { ActiveTexture = CalmTexture; }

void Enemy::OnBattlePhaseChange(BattlePhase b)
{
	switch (b)
	{
	case BattlePhase::Sighting:
	{
		SwitchToCalmTex();
		DrawMe = true;
	}
	break;
    case BattlePhase::Start:
    {
        SwitchToAttackTex(); 
        BattlePosition = Position;
    }
    break;
	case BattlePhase::End:
	{
        DamageMe = false;
        FadeOut = true;
        HealthPoints = MaxHealth;
	}
	break;
	}
}

void Enemy::Fade(float delta)
{
    static float timePerFrame = 0.05;
    static float elapsed = 0.0f;

    elapsed += delta;

    if (elapsed > timePerFrame)
    {
		Alpha -= 0.1;
        elapsed = 0.0;
    }
    if (Alpha <= 0)
    {
        FadeOut = false;
        DrawMe = false;
		SwitchToCalmTex();
    }
}

void Enemy::Shake(float delta)
{
    static float timePerFrame = 0.02;
    static float shakeAmount = 0.005f;
    static int shakeCount = 0;
    static float elapsed = timePerFrame;

    elapsed += delta;

    if (shakeCount > 5)
    {
        DamageMe = false;
        shakeCount = 0;
        return;
    }
    if (elapsed > timePerFrame)
    {
		Position.y += shakeAmount;
        shakeAmount = -shakeAmount;
        elapsed = 0.0;
        shakeCount++;
    }

	std::cout << Position.y << std::endl;
}

void Enemy::OnEnemyDamage(float dmgPercent)
{
    DamageMe = true;
}

void Enemy::OnMoveDistanceChange(float distance)
{
    if (DrawMe)
		Alpha = (distance == 0) ? 1.0f : distance;
}
