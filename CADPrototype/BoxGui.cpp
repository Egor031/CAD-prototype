#include "BoxGui.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>
#include <stdio.h>

#include "CreateBox.h"

#include <memory>

#include "History.h"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include <vtk_glad.h>

#include <string>


static char Input1[100] = "50",
Input2[100] = "50",
Input3[100] = "50",
Input4[100] = "50",
Input5[100] = "50",
Input6[100] = "50";

void BoxGui::Init(GLFWwindow* window)
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


static bool Flag1=1;

void BoxGui::Draw(History& history, Document& doc)
{
    Flag1 = 1;
    ImGui::Begin("Box");
    ImGui::InputText("Start point X", Input1, 50);
    ImGui::InputText("Start point Y", Input2, 50);
    ImGui::InputText("Start point Z", Input3, 50);
    ImGui::InputText("Lenght", Input4, 50);
    ImGui::InputText("Width", Input5, 50);
    ImGui::InputText("Height", Input6, 50);
    if (ImGui::Button("CreateBox")) { 
        
        double intInX = std::stoi(Input1);
        double intInY = std::stoi(Input2);
        double intInZ = std::stoi(Input3);
        double intLen = std::stoi(Input4);
        double intWid = std::stoi(Input5);
        double intHei = std::stoi(Input6);
        
        history.Apply(std::make_unique <CreateBox>(intInX, intInY, intInZ, intLen, intWid, intHei), doc); Flag1 = 0;}
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) Flag1=0;
    ImGui::End();

}

bool BoxGui::DoDraw()
{
    return (Flag1);
}

void BoxGui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
