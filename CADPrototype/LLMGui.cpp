#include "LLMGui.h"
#include "CommandFactory.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>
#include <stdio.h>
#include <AIS_Shape.hxx>

#include "Fuse.h"

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "LmStudioClient.h"
#include <vtk_glad.h>

#include <string>


void LLMGui::Init(GLFWwindow* window)
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

void LLMGui::Shutdown()
{
    if (!myInitialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    myInitialized = false;
}

void LLMGui::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}



char input[100] = "0";



static bool Flag1 = 1;
static bool SelectedF = 0;
static bool SelectedS = 0;

static bool initSellF = 0;
static bool initSellS = 0;

static int id1 = 0, id2 = 0;

void LLMGui::Draw(History& history, Document& doc)
{

   

    Flag1 = 1;
    ImGui::Begin("LLM");

    ImGui::InputText("LLM Request", input, 200);
    if (ImGui::Button("Run Model"))
    {
        //
        //
        //

        {

            static std::string myPrompt(input);
            const std::string state = doc.ExportStateJson();

            /*
            
            
            [
  {"type":"DrawBox","id":1,"dx":50,"dy":50,"dz":50,"Len":50,"Wid":50,"Hei":50,},
  {"type":"DrawCyll","id":2,"dx":0,"dy":0,"dz":0,"Dia":10,"Hei":20,"AxX":0,"AxY":0,"AxZ":1,},
  {"type":"DrawFuse","id":3,"Fid1":1,"Fid2":2,}
]
            * 
            * 
            * 
            *
            */

            std::string systemPrompt =
                "You are an assistant for creating 3D solid and simple CAD geometry.\n"
                "Your task is to convert the user's natural language request into commands.\n"
                "\n"
                "Return ONLY a valid JSON array of commands.\n"
                "No explanations. No markdown. No code fences. No comments.\n"
                "\n"
                "Use the current CAD state to choose existing object ids when the user asks to fuse or cut objects.\n"
                "If the user asks to create a new object, do not invent an id unless the command type explicitly requires id.\n"
                "\n"
                "Supported 3D solid command types:\n"
                "- AddBox {\"type\":\"DrawBox\", \"dx\":number, \"dy\":number, \"dz\":number,\"Len\":number,\"Wid\":number, \"Hei\":number}\n"
                "- AddCyll {\"type\":\"DrawCyll\", \"dx\":number, \"dy\":number, \"dz\":number, \"Dia\":number, \"Hei\":number, \"Axx\":number, \"Axy\":number, \"Axz\":number}\n"
                "- AddFuse {\"type\":\"DrawFuse\", \"Fid1\":number, \"Fid2\":number}\n"
                "- AddCut {\"type\":\"DrawCut\", \"Fid1\":number, \"Fid2\":number}\n"
                "\n"
                "Rules:\n"
                "1. For making solid bodys use commands AddBox, AddCyll, AddFuse, AddCut.\n"
                "2. Coordinates are in the XYZ coordinate system.\n"
                "3. A parallelepiped (Box) is defined by its point in negative dx,dy,dz direction and its width Wid, height Hei and lenght Len.\n"
                "4. A cyllinder (Cyll) is defined by center dx,dy,dz, diametr Dia, height Hei and vector from 0,0,0 point to Axx,Axy,Axz.\n"
                "5. If the user says Fuse or Cut, use id 1 as main object and id 2 as sub object tool.\n"
                "6. If the user says 'center', use the geometric center of the referenced object.\n"
                "8. If the request is ambiguous, make a reasonable simple assumption and still return JSON.\n"
                "\n"
                "Examples:\n"
                "User: Make Box from 50 50 50 with size 50 50 50.\n"
                "Answer: [{\"type\":\"DrawBox\",\"dx\":50,\"dy\":50,\"dz\":50,\"Len\":50,\"Wid\":50}]\n"
                "\n"
                "User: Make Box from 51 52 53 to 122, 124, 126.\n"
                "Answer: [{\"type\":\"DrawBox\",\"dx\":51,\"dy\":52,\"dz\":53,\"Len\":81,\"Wid\":82,\"Hei\":83}]\n"
                "\n"
                "Examples:\n"
                "User: box 50 50 50 size 50 50 50 and cyll 75 75 50 dia 10 hei 100 at z axis.\n"
                "Answer: [{\"type\":\"DrawBox\",\"dx\":50,\"dy\":50,\"dz\":50,\"Len\":50,\"Wid\":50},{\"type\":\"DrawCyll\",\"dx\":75,\"dy\":75,\"dz\":50,\"Dia\":10,\"Hei\":100,\"Axx\":0,\"Axy\":0,\"Axz\":1}]\n"
                "\n"
                "User: fuse box 50 50 50 size 50 50 50 and cyll 75 75 50 dia 10 hei 100 at z axis.\n"
                "Answer: [{\"type\":\"DrawBox\",\"dx\":50,\"dy\":50,\"dz\":50,\"Len\":50,\"Wid\":50},{\"type\":\"DrawCyll\",\"dx\":75,\"dy\":75,\"dz\":50,\"Dia\":10,\"Hei\":100,\"Axx\":0,\"Axy\":0,\"Axz\":1},{\"type\":\"DrawFuse\",\"Fid1\":1,\"Fid2\":2}]\n";

            std::string userPrompt =
                "User request:\n" + myPrompt + "\n\n"
                "Current CAD state (JSON):\n" + state + "\n\n"
                "Return ONLY JSON array of commands.";

            auto res = LmStudioClient::ChatCompletions(
                myEndpoint,
                myModel,
                systemPrompt,
                userPrompt,
                0.1,
                512
            );

            myLlmRequest = res.requestJson;
            myLlmRaw = res.rawResponseJson;
            myLlmResponse = res.content;

            std::string err;
            auto cmds = CommandFactory::ParseCommandArray(myLlmResponse, err);
            if (!err.empty())
            {
                myLlmError = err;
            }
            else
            {
                for (auto& c : cmds)
                    history.Apply(std::move(c), doc);

                myHistoryJson = history.ExportJson();
                myStateJson = doc.ExportStateJson();
            }

            ///
            ///
            /// 
        }

    }


    ImGui::End();

}

bool LLMGui::DoDraw()
{
    return (Flag1);
}

void LLMGui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
