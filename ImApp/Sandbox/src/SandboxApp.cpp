#include "ImApp/Application.h"
#include "ImApp/EntryPoint.h"

class ExampleLayer : public ImApp::Layer
{
public:
	virtual void OnUIRender() override
	{

		ImGui::ShowDemoWindow();
	}
};

ImApp::Application* ImApp::CreateApplication(int argc, char** argv)
{
	ImApp::ApplicationSpecification spec;
	spec.Name = "ImApp Example";

	ImApp::Application* app = new ImApp::Application(spec);
	app->PushLayer<ExampleLayer>();
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