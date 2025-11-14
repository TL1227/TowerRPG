#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::GetView()
{
	return glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void Camera::UpdateCameraRotation()
{
	CameraFront.x = cos(glm::radians(HorRot));
	CameraFront.z = sin(glm::radians(HorRot));
}
