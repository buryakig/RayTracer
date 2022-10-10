#pragma once
#define GLM_SWIZZLE
#include "ImApp/Application.h"
#include <glm/glm.hpp>
#include <vector>

struct SpacialData
{
	glm::vec4 Position	{ 0.0f, 0.0f, 5.0f, 0.0f };
	glm::vec4 Forward	{ 0.0f, 0.0f, -1.0f, 0.0f };
	glm::vec4 Up		{ 0.0f, 1.0f, 0.0f, 0.0f };
	glm::vec4 Right		{ 1.0f, 0.0f, 0.0f, 0.0f };
};

class Camera
{
public:

	Camera() {}

	void OnUpdate(float dt);

	float verticalFoV	{60.0};
	float nearClip		{0.0};
	float farClip		{0.0};

	SpacialData transform;

	uint32_t ViewportHeight	{ 0 };
	uint32_t ViewportWidth	{ 0 };

};

void HandleThirdPersonMovement(std::unique_ptr<Camera>& cam, float dt);