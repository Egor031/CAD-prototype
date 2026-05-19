#include "FuseGui.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>
#include <stdio.h>
#include <AIS_Shape.hxx>

#include "Fuse.h"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include <vtk_glad.h>

#include <string>


void FuseGui::Init(GLFWwindow* window)
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

void FuseGui::Shutdown()
{
    if (!myInitialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    myInitialized = false;
}

void FuseGui::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


static bool Flag1 = 1;
static bool SelectedF = 0;
static bool SelectedS = 0;

static bool initSellF = 0;
static bool initSellS = 0;

static int id1 = 0, id2 = 0;

void FuseGui::Draw(History& history, Document& doc)
{

    

    Flag1 = 1;
    ImGui::Begin("Fuse");

    if (!SelectedF)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.8f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.8f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.8f, 0.5f));
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.16f, 0.8f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.16f, 0.8f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.16f, 0.8f, 0.6f));

    }


    if (ImGui::Button("Select First Object"))
    {
        if (SelectedS == 1) id2 = doc.GetSelectedIDShape();
        SelectedF = 1; SelectedS = 0;
       // SOBJ = doc.GetSelectedTPDSShape();
    }

   ImGui::PopStyleColor(3);
    

   //
   //
   //


    if (!SelectedS)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.8f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.8f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.8f, 0.5f));
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.16f, 0.8f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.16f, 0.8f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.16f, 0.8f, 0.6f));

    }
    
    if (ImGui::Button("Select Second Object"))
    {
        if (SelectedF == 1) 
            id1 = doc.GetSelectedIDShape();
        SelectedF = 0; SelectedS = 1;
       // FOBJ = doc.GetSelectedTPDSShape();
    }
    

    ImGui::PopStyleColor(3);
    

    if (ImGui::Button("Fuse")) { 
        if (SelectedF == 1) id1 = doc.GetSelectedIDShape();
        if (SelectedS == 1) id2 = doc.GetSelectedIDShape();
        SelectedF = 0;SelectedS = 0;
        Flag1 = 0; history.Apply(std::make_unique <CreateFuse>(id1, id2), doc); initSellF = 0;initSellS = 0;    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        Flag1 = 0; SelectedF = 0;SelectedS = 0;
    }
    ImGui::End();

}

bool FuseGui::DoDraw()
{
    return (Flag1);
}

void FuseGui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
