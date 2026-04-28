#include "BoxGui.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>
#include <stdio.h>

#include "CreateBox.h"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include <vtk_glad.h>

#include <string>


static char Input1[100] = "Start X",
Input2[100] = "Start Y",
Input3[100] = "Start Z",
Input4[100] = "Lenght",
Input5[100] = "Width",
Input6[100] = "Height";

void BoxGui::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // === ВАЖНО: поддержка кириллицы ===
    ImFontConfig cfg;
    cfg.OversampleH = 3;
    cfg.OversampleV = 3;
    cfg.PixelSnapH = false;

    io.Fonts->AddFontFromFileTTF(
        "C:/Windows/Fonts/arial.ttf",
        18.0f,
        &cfg,
        io.Fonts->GetGlyphRangesCyrillic()
    );

    // Backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void BoxGui::Shutdown()
{
    if (!myInitialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    myInitialized = false;
}

void BoxGui::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


static int t = 0;
void BoxGui::Draw()
{
  
    ImGui::Begin("Box");
    ImGui::InputText("Start point X", Input1, 50);
    ImGui::InputText("Start point Y", Input2, 50);
    ImGui::InputText("Start point Z", Input3, 50);
    ImGui::InputText("Lenght", Input4, 50);
    ImGui::InputText("Width", Input5, 50);
    ImGui::InputText("Height", Input6, 50);
    if(ImGui::Button("CreateBox")) CreateBox.DrawBox(Input1, Input2, Input3, Input4, Input5, Input6 );
    ImGui::End();

}


void BoxGui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
