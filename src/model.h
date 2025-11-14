#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "camera.h"

class Model
{
public:
	std::vector<float> Verticies;
    unsigned int VAO;
    unsigned int ArrayBuffer;
    unsigned int VertBuffer;
    unsigned int Shader;

    Model(std::vector<float> verts, unsigned int shader);

	void Draw(Camera &camera, glm::vec3 translate);
    void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

#endif

