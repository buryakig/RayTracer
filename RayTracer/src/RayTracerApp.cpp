#include "ImApp/Application.h"
#include "ImApp/EntryPoint.h"
#include "Utils/Resources/Resources.h"
#include "RenderTexture.h"
#include <random>

void MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

class Mandelbrot : public ImApp::Layer
{
public:
	virtual void OnAttach() override
	{
		mainTex = std::unique_ptr<RenderTexture>(new RenderTexture(0, 0));

		testCompute = Resources::LoadComputeShader("RayTracer/res/ComputeShaders/raytracer.comp");
	
		glUniform1i(glGetUniformLocation(testCompute->programID, "destTex"), 0);

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

			DrawMandelbrotCompute();

			ImGui::Image((void*)(intptr_t)mainTex->id, ImVec2(m_ViewportWidth, m_ViewportHeight));

		ImGui::End();
		ImGui::PopStyleVar();

		//ImGui::ShowDemoWindow();

	}

	void DrawMandelbrotCompute()
	{
		if (m_ViewportWidth != mainTex->width || m_ViewportHeight != mainTex->height)
		{
			mainTex = std::unique_ptr<RenderTexture>(new RenderTexture(m_ViewportWidth, m_ViewportHeight));
			std::cout << "Main texture recreated.\n";
		}

		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glUseProgram(testCompute->programID);
		UpdateComputeVariables(testCompute->programID);

		glDispatchCompute(ceil(m_ViewportWidth / 16.0), ceil(m_ViewportHeight / 16.0), 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}

	void UpdateComputeVariables(int programID)
	{
		glUniform4f(glGetUniformLocation(programID, "imageSize"),
			m_ViewportWidth, m_ViewportHeight, 1.0 / m_ViewportWidth, 1.0 / m_ViewportHeight);
		glUniform1f(glGetUniformLocation(programID, "roughness_right"), (float)roughness);

	}


	std::shared_ptr<ComputeShader> testCompute;
	std::unique_ptr<RenderTexture> mainTex;
	
	float roughness = 0.9;

	glm::vec2 cParam;

	int m_ViewportWidth = 0, m_ViewportHeight = 0;
};

ImApp::Application* ImApp::CreateApplication(int argc, char** argv)
{
	ImApp::ApplicationSpecification spec;
	spec.Name = "Raytracer";
	spec.Width = 1280;
	spec.Height = 900;

	ImApp::Application* app = new ImApp::Application(spec);
	app->PushLayer<Mandelbrot>();
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