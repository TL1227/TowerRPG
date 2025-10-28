#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float HorRot = 0.0f;
};

#endif
