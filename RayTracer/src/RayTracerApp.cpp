#include "ImApp/Application.h"
#include "ImApp/EntryPoint.h"
#include "Utils/Resources/Resources.h"
#include "RenderTexture.h"
#include "Camera.h"
#include <random>

void MessageCallback(GLenum source,
	const GLenum type,
	GLuint id,
	const GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* user_param)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

class Renderer : public ImApp::Layer
{
public:
	virtual void OnAttach() override
	{
		mainTex = std::make_unique<RenderTexture>(0, 0);
		mainCamera = std::make_unique<Camera>();

		test_compute = Resources::LoadComputeShader("RayTracer/res/ComputeShaders/raytracer.comp");
	
		glUniform1i(glGetUniformLocation(test_compute->programID, "destTex"), 0);
		
		// Create Uniform Buffer for camera
		glGenBuffers(1, &cameraBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, cameraBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &mainCamera->transform);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	virtual void OnUpdate(float dt) override
	{
		HandleThirdPersonMovement(mainCamera, dt);

		glBindBufferBase(GL_UNIFORM_BUFFER, 1, cameraBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &mainCamera->transform);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		
			if (ImGui::CollapsingHeader("Log variables", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("mainTex pointer = %p", mainTex->id);
				ImGui::Text("Viewport size = %d x %d", m_ViewportWidth, m_ViewportHeight);
			}
			if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("Roughness right", &roughness, 0.0f, 1.0f, "ratio = %.3f");
			}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Viewport");

			m_ViewportWidth = ImGui::GetContentRegionAvail().x;
			m_ViewportHeight = ImGui::GetContentRegionAvail().y;

			DrawRaytracerBuffer();

			ImGui::Image((void*)(intptr_t)mainTex->id, ImVec2(m_ViewportWidth, m_ViewportHeight));

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void DrawRaytracerBuffer()
	{
		if (m_ViewportWidth != mainTex->width || m_ViewportHeight != mainTex->height)
		{
			mainTex = std::make_unique<RenderTexture>(m_ViewportWidth, m_ViewportHeight);
			std::cout << "Main texture recreated.\n";
		}

		//glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glUseProgram(test_compute->programID);
		UpdateComputeVariables(test_compute->programID);

		glDispatchCompute(ceil(m_ViewportWidth / 16.0), ceil(m_ViewportHeight / 16.0), 1);
		//glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}

	void UpdateComputeVariables(const int programID) const
	{
		glUniform4f(glGetUniformLocation(programID, "imageSize"),
			m_ViewportWidth, m_ViewportHeight, 1.0f / m_ViewportWidth, 1.0f / m_ViewportHeight);
		glUniform1f(glGetUniformLocation(programID, "roughness_right"), (float)roughness);

	}

	std::shared_ptr<ComputeShader> test_compute;
	std::unique_ptr<RenderTexture> mainTex;
	std::unique_ptr<Camera> mainCamera;
	
	float roughness = 0.9;

	glm::vec2 cParam;

	int m_ViewportWidth = 0, m_ViewportHeight = 0;

	unsigned int cameraBuffer = 0;
};

ImApp::Application* ImApp::CreateApplication(int argc, char** argv)
{
	ImApp::ApplicationSpecification spec;
	spec.Name = "Raytracer";
	spec.Width = 1280;
	spec.Height = 900;

	auto* app = new ImApp::Application(spec);
	app->PushLayer<Renderer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}