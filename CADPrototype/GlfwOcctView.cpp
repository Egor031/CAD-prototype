#include "GlfwOcctView.h"

#include <AIS_Shape.hxx>
#include <AIS_ViewCube.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <OpenGl_GraphicDriver.hxx>

#include <iostream>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include "CmdAddBox.h"
#include <memory>
#include <utility>
#include "CommandFactory.h"

GlfwOcctView::GlfwOcctView() {}
GlfwOcctView::~GlfwOcctView() {}

GlfwOcctView* GlfwOcctView::toView(GLFWwindow* theWin)
{
    return static_cast<GlfwOcctView*>(glfwGetWindowUserPointer(theWin));
}

void GlfwOcctView::errorCallback(int theError, const char* theDescription)
{
    Message::DefaultMessenger()->Send(
        TCollection_AsciiString("Error") + theError + ": " + theDescription, Message_Fail);
}

void GlfwOcctView::run()
{
    initWindow(800, 600, "OCCT IMGUI");
    initViewer();

    if (myView.IsNull() || myContext.IsNull() || myOcctWindow.IsNull())
    {
        return;
    }

    myController = std::make_unique<OcctInputController>(myView, myContext, myOcctWindow);

    myCad = std::make_unique<CadSession>(myContext);

    myDocument = std::make_unique<Document>(myContext);

    initViewCube();
    initDemoScene();

    myView->MustBeResized();
    myOcctWindow->Map();

    myGui.Init(myOcctWindow->getGlfwWindow());
    mainloop();
    myGui.Shutdown();
    cleanup();

}

void GlfwOcctView::initWindow(int theWidth, int theHeight, const char* theTitle)
{
    glfwSetErrorCallback(GlfwOcctView::errorCallback);
    glfwInit();

    const bool toAskCoreProfile = true;
    if (toAskCoreProfile)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#if defined (__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    myOcctWindow = new GlfwOcctWindow(theWidth, theHeight, theTitle);
    glfwSetWindowUserPointer(myOcctWindow->getGlfwWindow(), this);

    glfwSetWindowSizeCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onResizeCallback);
    glfwSetFramebufferSizeCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onFBResizeCallback);

    glfwSetScrollCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseScrollCallback);
    glfwSetMouseButtonCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseButtonCallback);
    glfwSetCursorPosCallback(myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseMoveCallback);
}

void GlfwOcctView::initViewer()
{
    if (myOcctWindow.IsNull() || myOcctWindow->getGlfwWindow() == nullptr)
    {
        return;
    }

    Handle(OpenGl_GraphicDriver) aGraphicDriver =
        new OpenGl_GraphicDriver(myOcctWindow->GetDisplay(), Standard_False);
    aGraphicDriver->SetBuffersNoSwap(Standard_True);

    Handle(V3d_Viewer) aViewer = new V3d_Viewer(aGraphicDriver);
    aViewer->SetDefaultLights();
    aViewer->SetLightOn();
    aViewer->SetDefaultTypeOfView(V3d_PERSPECTIVE);
    aViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);

    myView = aViewer->CreateView();
    myView->SetWindow(myOcctWindow, myOcctWindow->NativeGlContext());
    myView->ChangeRenderingParams().ToShowStats = Standard_True;

    myContext = new AIS_InteractiveContext(aViewer);
}

void GlfwOcctView::initViewCube()
{
    if (myContext.IsNull() || !myController)
    {
        return;
    }

    Handle(AIS_ViewCube) aCube = new AIS_ViewCube();
    aCube->SetSize(55);
    aCube->SetFontHeight(12);
    aCube->SetAxesLabels("", "", "");
    aCube->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers, Aspect_TOTP_LEFT_LOWER, Graphic3d_Vec2i(100, 100)));
    aCube->SetViewAnimation(myController->ViewAnimation());
    aCube->SetFixedAnimationLoop(false);

    myContext->Display(aCube, false);
}


void GlfwOcctView::initDemoScene()
{
    if (!myCad) return;

    //myCad->AddBox(50, 50, 50);
    //myCad->AddCone(25, 0, 50);
}


void GlfwOcctView::mainloop()
{
    while (!glfwWindowShouldClose(myOcctWindow->getGlfwWindow()))
    {
        //if (myController && myController->ToWaitEvents()) glfwWaitEvents();
        //else glfwPollEvents();
        glfwPollEvents();

        myController->Flush();

        myGui.BeginFrame();

        ImGui::Begin("CAD");

        ImGui::Button("Add Box");
        if (ImGui::IsItemActivated())
        {
            myHistory.Apply(std::make_unique<CmdAddBox>(50, 50, 50), *myDocument);
            myContext->UpdateCurrentViewer();
        }

        ImGui::Button("Undo");
        if (ImGui::IsItemActivated())
        {
            myHistory.Undo(*myDocument);
            myContext->UpdateCurrentViewer();
        }

        ImGui::Button("Redo");
        if (ImGui::IsItemActivated())
        {
            myHistory.Redo(*myDocument);
            myContext->UpdateCurrentViewer();
        }

        static std::string historyJson;
        static std::string importJson = R"([
  {"type":"AddBox","dx":50,"dy":50,"dz":50}
])";
        static std::string importError;

        if (ImGui::Button("Refresh History JSON"))
        {
            historyJson = myHistory.ExportJson();
        }

        ImGui::SameLine();
        if (ImGui::Button("Copy"))
        {
            ImGui::SetClipboardText(historyJson.c_str());
        }

        ImGui::BeginChild("history_box", ImVec2(520, 220), true);
        ImGui::TextUnformatted(historyJson.c_str());
        ImGui::EndChild();
        ImGui::Separator();
        ImGui::Text("Import JSON:");

        ImGui::BeginChild("import_box", ImVec2(520, 140), true);
        ImGui::TextUnformatted(importJson.c_str());
        ImGui::EndChild();

        // Чтобы можно было редактировать — лучше использовать InputTextMultiline с буфером.
        // Простой вариант без буфера: сделай отдельное окно-редактор позже.
        // Сейчас можно просто вставлять через SetClipboardText/ручной источник.

        if (ImGui::Button("Apply JSON"))
        {
            importError.clear();

            std::string err;
            auto cmds = CommandFactory::ParseCommandArray(importJson, err);
            if (!err.empty())
            {
                importError = err;
            }
            else
            {
                for (auto& c : cmds)
                {
                    myHistory.Apply(std::move(c), *myDocument);
                }
                myContext->UpdateCurrentViewer();
            }
        }

        if (!importError.empty())
        {
            ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "%s", importError.c_str());
        }



        ImGui::End();

        myGui.Draw();
        myGui.EndFrame();
        glfwSwapBuffers(myOcctWindow->getGlfwWindow());


        
    }
}

void GlfwOcctView::cleanup()
{
    if (!myView.IsNull())
    {
        myView->Remove();
    }
    if (!myOcctWindow.IsNull())
    {
        myOcctWindow->Close();
    }

    glfwTerminate();
}

// теперь обработчики только прокидывают события в контроллер

void GlfwOcctView::onResize(int theWidth, int theHeight)
{
    if (myController)
        myController->OnResize(theWidth, theHeight);
}

void GlfwOcctView::onMouseScroll(double theOffsetX, double theOffsetY)
{
    if (myController)
        myController->OnMouseScroll(theOffsetX, theOffsetY);
}

void GlfwOcctView::onMouseButton(int theButton, int theAction, int theMods)
{
    if (myController)
        myController->OnMouseButton(theButton, theAction, theMods);
}

void GlfwOcctView::onMouseMove(int thePosX, int thePosY)
{
    if (myController)
        myController->OnMouseMove(thePosX, thePosY);
}
