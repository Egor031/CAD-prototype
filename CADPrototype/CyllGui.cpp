#include "CyllGui.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>
#include <stdio.h>

#include "CreateCyll.h"
#include "history.h"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include <vtk_glad.h>

#include <string>


static char Input1[100] = "0",
Input2[100] = "0",
Input3[100] = "0",
Input4[100] = "10",
Input5[100] = "20",
Input6[100] = "0",
Input7[100] = "0",
Input8[100] = "1";

void CyllGui::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void CyllGui::Shutdown()
{
    if (!myInitialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    myInitialized = false;
}

void CyllGui::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


static bool Flag1 = 1;

void CyllGui::Draw(History& history, Document& doc)
{
    Flag1 = 1;
    ImGui::Begin("Cyllinder");
    ImGui::InputText("Center point X", Input1, 50);
    ImGui::InputText("Center point Y", Input2, 50);
    ImGui::InputText("Center point Z", Input3, 50);
    ImGui::InputText("Diametr", Input4, 50);
    ImGui::InputText("Height", Input5, 50);
    ImGui::InputText("Axis multiply X", Input6, 50);
    ImGui::InputText("Axis multiply Y", Input7, 50);
    ImGui::InputText("Axis multiply Z", Input8, 50);
    if (ImGui::Button("CreateCyll")) {
        
        double intInX = std::stoi(Input1);
        double  intInY = std::stoi(Input2);
        double  intInZ = std::stoi(Input3);
        double  intDiam = std::stoi(Input4);
        double  intHei = std::stoi(Input5);
        double  intaxelX = std::stoi(Input6);
        double  intaxelY = std::stoi(Input7);
        double  intaxelZ = std::stoi(Input8);
            history.Apply(std::make_unique <CreateCyll>(intInX, intInY, intInZ, intDiam, intHei, intaxelX,intaxelY,intaxelZ), doc);
        Flag1 = 0;
    
    ;Flag1 = 0;}
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) Flag1 = 0;
    ImGui::End();

}

bool CyllGui::DoDraw()
{
    return (Flag1);
}

void CyllGui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
