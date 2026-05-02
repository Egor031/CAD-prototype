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
static bool DrawBoxGui=0;
static bool DrawCyllGui = 0;
static bool DrawFuseGui = 0;
void Tools::Draw(Document& doc)
{

    ImGui::Begin("Tools");
    std::string File1 = R"(D:\Learn\Diplom\PNG\FirstBTNFrame.png)";
    std::string File2 = R"(D:\Learn\Diplom\PNG\SecondBTNFrame.png)";
    std::string File3 = R"(D:\Learn\Diplom\PNG\BoxPng.png)";
    std::string File4 = R"(D:\Learn\Diplom\PNG\CyllPng.png)";
    std::string File5 = R"(D:\Learn\Diplom\PNG\FusePng.png)";
    std::string File6 = R"(D:\Learn\Diplom\PNG\Trash.png)";

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImVec2 ToolsBtnSize(64, 64);
    if (ImGui::ImageButton("FirstPNGBtn", PNG_Reader.PNG_Read_ImGuiTex(File1), ToolsBtnSize)) count++;
    ImGui::SameLine();
    if (ImGui::ImageButton("SecondPNGBtn", PNG_Reader.PNG_Read_ImGuiTex(File2), ToolsBtnSize)) count=0;
    ImGui::SameLine();
    if (ImGui::ImageButton("BoxPng", PNG_Reader.PNG_Read_ImGuiTex(File3), ToolsBtnSize)) DrawBoxGui = 1;
    ImGui::SameLine();
    if (ImGui::ImageButton("CyllPng", PNG_Reader.PNG_Read_ImGuiTex(File4), ToolsBtnSize)) DrawCyllGui = 1;
    ImGui::SameLine();
    if (ImGui::ImageButton("FusePng", PNG_Reader.PNG_Read_ImGuiTex(File5), ToolsBtnSize)) DrawFuseGui = 1;
    ImGui::SameLine();
    if (ImGui::ImageButton("Trash", PNG_Reader.PNG_Read_ImGuiTex(File6), ToolsBtnSize)) doc.TempDell();
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::Text("%d", count);
    if (DrawBoxGui)
    {
        BoxGui.Draw(doc);
        if (!BoxGui.DoDraw()) DrawBoxGui = 0;
    }
    if (DrawCyllGui)
    {
        CyllGui.Draw(doc);
        if (!CyllGui.DoDraw()) DrawCyllGui = 0;
    }
    if (DrawFuseGui)
    {
        FuseGui.Draw(doc);
        if (!FuseGui.DoDraw()) DrawFuseGui = 0;
    }
    ImGui::End();
    
}


void Tools::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
