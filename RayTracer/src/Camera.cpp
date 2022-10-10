#include "Camera.h"

#include "ImApp/Input/Input.h"
#include <glm/gtx/quaternion.hpp>

glm::vec2 lastMousePos{ 0.0f };
float totalPitch = 0.0f;

void Camera::OnUpdate(float dt)
{

}

void HandleThirdPersonMovement(std::unique_ptr<Camera>& cam, float dt)
{
	const float speed = 5.0f;
	const float rotSpeed = 1.5f;

	glm::vec2 mousePos = ImApp::Input::GetMousePos();
	glm::vec2 delta = (mousePos - lastMousePos) * 0.002f;
	lastMousePos = mousePos;

	if (!ImApp::Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_2))
	{
		ImApp::Input::SetCursorMode(0);
		return;
	}

	ImApp::Input::SetCursorMode(2);

	bool moved = false;

	constexpr glm::vec4 globalUpAxis(0.0f, 1.0f, 0.0f, 0.0f);
	//constexpr glm::vec3 globalUpAxis(0.0f, 1.0f, 0.0f);

	// Movement
	if (ImApp::Input::IsKeyDown(GLFW_KEY_W))
	{
		cam->transform.Position += cam->transform.Forward * speed * dt;
		moved = true;
	}
	if (ImApp::Input::IsKeyDown(GLFW_KEY_S))
	{
		cam->transform.Position -= cam->transform.Forward * speed * dt;
		moved = true;
	}
	if (ImApp::Input::IsKeyDown(GLFW_KEY_A))
	{
		cam->transform.Position -= cam->transform.Right * speed * dt;
		moved = true;
	}
	if (ImApp::Input::IsKeyDown(GLFW_KEY_D))
	{
		cam->transform.Position += cam->transform.Right * speed * dt;
		moved = true;
	}
	if (ImApp::Input::IsKeyDown(GLFW_KEY_E))
	{
		cam->transform.Position -= globalUpAxis * speed * dt;
		moved = true;
	}
	if (ImApp::Input::IsKeyDown(GLFW_KEY_Q))
	{
		cam->transform.Position += globalUpAxis * speed * dt;
		moved = true;
	}

	// Rotation
	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitchDelta = delta.y* rotSpeed;
		if(totalPitch + pitchDelta > 3.1f)
		{
			pitchDelta = 3.1f - totalPitch;
			totalPitch = 3.1f;
		}
		else if (totalPitch + pitchDelta < -3.1f)
		{
			pitchDelta = -3.1f - totalPitch;
			totalPitch = -3.1f;
		}
		else
		{
			totalPitch += pitchDelta;
		}

		float yawDelta = delta.x * rotSpeed;

		glm::quat q = glm::normalize(cross(glm::angleAxis(-pitchDelta, cam->transform.Right.xyz()),
											glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		cam->transform.Forward = glm::vec4(glm::rotate(q, cam->transform.Forward.xyz()), 0.0f);

		cam->transform.Right = glm::normalize(glm::vec4(glm::cross(cam->transform.Forward.xyz(), globalUpAxis.xyz()), 0.0f));
		cam->transform.Up = glm::normalize(glm::vec4(glm::cross(cam->transform.Right.xyz(), cam->transform.Forward.xyz()), 0.0f));

		moved = true;
	}
}
