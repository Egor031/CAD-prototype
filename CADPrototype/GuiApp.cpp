#include "GuiApp.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>

#include <GLFW/glfw3.h>

void GuiApp::Init(GLFWwindow* theWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(theWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    myInitialized = true;
}

void GuiApp::Shutdown()
{
    if (!myInitialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    myInitialized = false;
}

void GuiApp::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiApp::Draw()
{
    // Временно оставляем твой GUI
    ImGui::ShowDemoWindow();

    ImGui::Begin("Hello");
    ImGui::Text("Hello ImGui!");
    ImGui::Text("Hello OpenCASCADE!");
    ImGui::Button("OK");
    ImGui::SameLine();
    ImGui::Button("Cancel");
    ImGui::End();
}

void GuiApp::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
