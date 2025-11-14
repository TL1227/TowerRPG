#include "model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "textures.h"

Model::Model(std::vector<float> verts, std::vector<unsigned int> indices, unsigned int shader)
	: Vertices{ verts }, Indices{ indices }, Shader { shader }
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    glGenBuffers(1, &ArrayBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ArrayBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,  Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &VertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertBuffer);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int wallTex = LoadTexture("textures\\stone.jpg");
    glUniform1i(glGetUniformLocation(shader, "texture1"), wallTex);
}

void Model::Draw(Camera &camera, glm::vec3 translate)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(translate.x, translate.y, translate.z));
	unsigned int modelLoc = glGetUniformLocation(Shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	camera.CameraFront.x = cos(glm::radians(camera.HorRot));
	camera.CameraFront.z = sin(glm::radians(camera.HorRot));
    glm::mat4 view = lookAt(camera.CameraPos, camera.CameraPos + camera.CameraFront, camera.CameraUp);
	unsigned int viewLoc = glGetUniformLocation(Shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
	unsigned int projectionLoc = glGetUniformLocation(Shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

    glUseProgram(Shader);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Model::Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	unsigned int modelLoc = glGetUniformLocation(Shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	unsigned int viewLoc = glGetUniformLocation(Shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

	unsigned int projectionLoc = glGetUniformLocation(Shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

    glUseProgram(Shader);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}
