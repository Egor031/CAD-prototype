#include "Tools.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>
#include <stdio.h>

#include "PNG_Reader.h"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <vtk_glad.h>

#include <string>

void Tools::Init(GLFWwindow* window)
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

void Tools::Shutdown()
{
    if (!myInitialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    myInitialized = false;
}

void Tools::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


static int count = 0;

void Tools::Draw()
{

    ImGui::Begin("Tools");
    std::string File1 = R"(D:\Learn\Diplom\Build1\FirstBTNFrame.bmp)";
    std::string File2 = R"(D:\Learn\Diplom\Build1\SecondBTNFrame.bmp)";

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImVec2 ToolsBtnSize(64, 64);
    if (ImGui::ImageButton("FirstPNGBtn", PNG_Reader.PNG_Read_ImGuiTex(File1), ToolsBtnSize)) count++;
    ImGui::SameLine();
    if (ImGui::ImageButton("SecondPNGBtn", PNG_Reader.PNG_Read_ImGuiTex(File2), ToolsBtnSize)) count=0;
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::Text("%d", count);
    ImGui::End();
    
}


void Tools::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
