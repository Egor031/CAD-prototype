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

std::string File1 = R"(D:\Learn\Diplom\PNG\FirstBTNFrame.png)";
std::string File2 = R"(D:\Learn\Diplom\PNG\SecondBTNFrame.png)";
std::string File3 = R"(D:\Learn\Diplom\PNG\BoxPng.png)";
std::string File4 = R"(D:\Learn\Diplom\PNG\CyllPng.png)";
std::string File5 = R"(D:\Learn\Diplom\PNG\FusePng.png)";
std::string File8 = R"(D:\Learn\Diplom\PNG\CutPng.png)";
std::string File6 = R"(D:\Learn\Diplom\PNG\Trash.png)";
std::string File7 = R"(D:\Learn\Diplom\PNG\LLM.png)";

static ImTextureRef Png1;
static ImTextureRef Png2;
static ImTextureRef Png3;
static ImTextureRef Png4;
static ImTextureRef Png5;
static ImTextureRef Png6;
static ImTextureRef Png7;
static ImTextureRef Png8;

void Tools::IniPNG()
{
    Png1 = PNG_Reader.PNG_Read_ImGuiTex(File1);
    Png2 = PNG_Reader.PNG_Read_ImGuiTex(File2);
    Png3 = PNG_Reader.PNG_Read_ImGuiTex(File3);
    Png4 = PNG_Reader.PNG_Read_ImGuiTex(File4);
    Png5 = PNG_Reader.PNG_Read_ImGuiTex(File5);
    Png6 = PNG_Reader.PNG_Read_ImGuiTex(File6);
    Png7 = PNG_Reader.PNG_Read_ImGuiTex(File7);
    Png8 = PNG_Reader.PNG_Read_ImGuiTex(File8);
}

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
static bool DrawCutGui = 0;
static bool DrawLLMGui = 0;

void Tools::Draw(History& history, Document& doc)
{

    ImGui::Begin("Tools");
   

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImVec2 ToolsBtnSize(64, 64);
    if (ImGui::ImageButton("FirstPNGBtn", Png1, ToolsBtnSize)) count++;
    ImGui::SameLine();
    if (ImGui::ImageButton("SecondPNGBtn", Png2, ToolsBtnSize)) count=0;
    ImGui::SameLine();
    if (ImGui::ImageButton("BoxPng", Png3, ToolsBtnSize)) DrawBoxGui = 1;
    ImGui::SameLine();
    if (ImGui::ImageButton("CyllPng", Png4, ToolsBtnSize)) DrawCyllGui = 1;
    ImGui::SameLine();
    if (ImGui::ImageButton("FusePng", Png5, ToolsBtnSize)) DrawFuseGui = 1;
    ImGui::SameLine();
    if (ImGui::ImageButton("CutPng", Png8, ToolsBtnSize)) DrawCutGui = 1;
    ImGui::SameLine();
    if (ImGui::ImageButton("Trash", Png6, ToolsBtnSize)) doc.TempDell();
    ImGui::SameLine();
    if (ImGui::ImageButton("LLM", Png7, ToolsBtnSize)) DrawLLMGui=1;

    /*

    if (ImGui::Button("Box")) DrawBoxGui = 1;
    if (ImGui::Button("Cyll")) DrawCyllGui = 1;
    if (ImGui::Button("Fuse")) DrawFuseGui = 1;
    if (ImGui::Button("LLM")) DrawLLMGui = 1;
    */
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    //ImGui::Text("%d", count);
    if (DrawBoxGui)
    {
        BoxGui.Draw(history, doc);
        if (!BoxGui.DoDraw()) DrawBoxGui = 0;
    }
    if (DrawCyllGui)
    {
        CyllGui.Draw(history, doc);
        if (!CyllGui.DoDraw()) DrawCyllGui = 0;
    }
    if (DrawFuseGui)
    {
        FuseGui.Draw(history, doc);
        if (!FuseGui.DoDraw()) DrawFuseGui = 0;
    }
    if (DrawCutGui)
    {
        CutGui.Draw(history, doc);
        if (!CutGui.DoDraw()) DrawCutGui = 0;
    }
    if (DrawLLMGui)
    {
        LLMGui.Draw(history, doc);
        if (!LLMGui.DoDraw()) DrawLLMGui = 0;
    }
    ImGui::End();
    
}


void Tools::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
