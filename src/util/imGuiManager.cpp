#include "ImGuiManager.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <ImGuizmo.h>

void setupImGuiStyle();

static ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

ImGuiManager::ImGuiManager(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer)
	: m_window(window), m_renderer(renderer), m_renderSize({ 800,600 }), m_renderAspectRatio(16.0f / 9.0f), m_shouldResize(false),
	m_renderPos({ 0,0 })
{

}

void ImGuiManager::init()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	setupImGuiStyle();

	// Adjust font size with DPI, set font to arial
	ImFont* font = io.Fonts->AddFontFromFileTTF("res/fonts/ubuntu.ttf", 24.0f);
	IM_ASSERT(font != NULL);

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window->get(), true);
	ImGui_ImplOpenGL3_Init("#version 430");
}


void ImGuiManager::draw(uint32_t texture)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	// Docking
	{
		// The main dockspace
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// Menu bar
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
					if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
					if (ImGui::MenuItem("Close", "Ctrl+W")) { /* Do stuff */ }
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

		auto& camera = m_renderer->getCamera();
		// Render scene
		ImGui::Begin("Scene");

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImVec2 windowPos = ImGui::GetWindowPos();
		if (windowSize.x != m_renderSize.x || windowSize.y != m_renderSize.y)
		{
			m_renderSize = { windowSize.x, windowSize.y };
			m_renderPos = { windowPos.x, windowPos.y };
			setRenderSize(m_renderSize);
			m_shouldResize = true;
		}
		ImGui::Image((void*)texture, windowSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(m_renderPos.x, m_renderPos.y, m_renderSize.x, m_renderSize.y);
		ImGui::End();

		// Light manager
		ImGui::Begin("Lights");

		if (ImGui::CollapsingHeader("Directional Light")) 
		{

			std::shared_ptr<DirectionalLight> dirLight = m_renderer->getLightManager().getDirectionalLight();
			glm::vec3 rot = dirLight->getRotation();
			glm::vec3 pos = dirLight->getPosition();

			m_drawGizmos(dirLight);

			if (ImGui::DragFloat3("Position", glm::value_ptr(pos)))
			{
				dirLight->setPosition(pos);
			}
			if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot)))
			{
				dirLight->setRotation(rot);
			}
			glm::vec3 color = dirLight->getColor();
			if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
			{
				dirLight->setColor(color);
			}
			float intensity = dirLight->getIntensity();
			if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 100.0f))
			{
				dirLight->setIntensity(intensity);
			}
			
		}

		if (ImGui::CollapsingHeader("Point Lights")) 
		{
			auto& pointLights = m_renderer->getLightManager().getPointLights();
			for (size_t i = 0; i < pointLights.size(); ++i) 
			{
				ImGui::Indent();
				ImGui::PushID(i); 

				if (ImGui::CollapsingHeader(("Point Light " + std::to_string(i)).c_str() ))
				{
					std::shared_ptr<PointLight> pointLight = pointLights[i];
					glm::vec3 pos = pointLight->getPosition();
					if (ImGui::DragFloat3("Position", glm::value_ptr(pos))) 
					{
						pointLight->setPosition(pos);
					}
					glm::vec3 color = pointLight->getColor();
					if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) 
					{
						pointLight->setColor(color);
					}
					float intensity = pointLight->getIntensity();
					if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 100.0f)) 
					{
						pointLight->setIntensity(intensity);
					}
					float constant = pointLight->getConstant();
					if (ImGui::DragFloat("Constant", &constant, 0.01f, 0.0f, 100.0f)) 
					{
						pointLight->setConstant(constant);
					}
					float linear = pointLight->getLinear();
					if (ImGui::DragFloat("Linear", &linear, 0.01f, 0.0f, 100.0f)) 
					{
						pointLight->setLinear(linear);
					}
					float quadratic = pointLight->getQuadratic();
					if (ImGui::DragFloat("Quadratic", &quadratic, 0.01f, 0.0f, 100.0f)) 
{
						pointLight->setQuadratic(quadratic);
					}
				}

				ImGui::PopID();
				ImGui::Unindent();
			}

			if (ImGui::Button("Add Point Light")) {
				// Assuming 'addLight' takes a shared_ptr to a new light as parameter
				m_renderer->getLightManager().addLight(std::make_shared<PointLight>(glm::vec3(0), glm::vec3(1), 1.0f, 1.0f, 0.09f, 0.032f));
			}

			ImGui::Text("Amount of Point Lights: %d", static_cast<int>(pointLights.size()));
		}

		ImGui::CollapsingHeader("Spot Light");
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiManager::shutdown()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::m_drawGizmos(std::shared_ptr<Light> light)
{
	auto& camera = m_renderer->getCamera();
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), light->getPosition());
	modelMatrix = glm::rotate(modelMatrix, glm::radians(light->getRotation().x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(light->getRotation().y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(light->getRotation().z), glm::vec3(0, 0, 1));
	ImGuizmo::DrawCubes(glm::value_ptr(camera.getViewMatrix()), glm::value_ptr(camera.getProjectionMatrix()), glm::value_ptr(modelMatrix), 1);

	// key press
	if (ImGui::IsKeyPressed(ImGuiKey_W))
	{
		currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

	}
	if (ImGui::IsKeyPressed(ImGuiKey_R))
	{
		currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
	}

	ImGuizmo::Manipulate(glm::value_ptr(camera.getViewMatrix()), glm::value_ptr(camera.getProjectionMatrix()),
		currentGizmoOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(modelMatrix));
	
	if (ImGuizmo::IsUsing() && currentGizmoOperation == ImGuizmo::OPERATION::TRANSLATE)
	{
		glm::vec3 newPosition;
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), glm::value_ptr(newPosition), nullptr, nullptr);
		light->setPosition(newPosition);
	}

	if (ImGuizmo::IsUsing() && currentGizmoOperation == ImGuizmo::OPERATION::ROTATE)
	{
		glm::vec3 newRotation;
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), nullptr, glm::value_ptr(newRotation), nullptr);
		light->setRotation(newRotation);
	}
}

void setupImGuiStyle()
{
	// Discord (Dark) style by BttrDrgn from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2117647081613541f, 0.2235294133424759f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1843137294054031f, 0.1921568661928177f, 0.2117647081613541f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3098039329051971f, 0.3294117748737335f, 0.3607843220233917f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3098039329051971f, 0.3294117748737335f, 0.3607843220233917f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3450980484485626f, 0.3960784375667572f, 0.9490196108818054f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1843137294054031f, 0.1921568661928177f, 0.2117647081613541f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.125490203499794f, 0.1333333402872086f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.125490203499794f, 0.1333333402872086f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.125490203499794f, 0.1333333402872086f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2313725501298904f, 0.6470588445663452f, 0.364705890417099f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.3098039329051971f, 0.3294117748737335f, 0.3607843220233917f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.407843142747879f, 0.4274509847164154f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.125490203499794f, 0.1333333402872086f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.3098039329051971f, 0.3294117748737335f, 0.3607843220233917f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.407843142747879f, 0.4274509847164154f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.407843142747879f, 0.4274509847164154f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1843137294054031f, 0.1921568661928177f, 0.2117647081613541f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2352941185235977f, 0.2470588237047195f, 0.2705882489681244f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.2588235437870026f, 0.2745098173618317f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.3450980484485626f, 0.3960784375667572f, 0.9490196108818054f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.3450980484485626f, 0.3960784375667572f, 0.9490196108818054f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.3607843220233917f, 0.4000000059604645f, 0.4274509847164154f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.05098039284348488f, 0.4196078479290009f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.3450980484485626f, 0.3960784375667572f, 0.9490196108818054f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}