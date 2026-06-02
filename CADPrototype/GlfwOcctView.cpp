#include "GlfwOcctView.h"


#include <AIS_ViewCube.hxx>
#include <Aspect_DisplayConnection.hxx>

#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <OpenGl_GraphicDriver.hxx>

#include <iostream>

#include <GLFW/glfw3.h>

#include "Document.h"

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



    myDocument = std::make_unique<Document>(myContext);

    initViewCube();
    //initDemoScene();

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


//void GlfwOcctView::initDemoScene()
//{
//    if (!myCad) return;
//
//    //myCad->AddBox(50, 50, 50);
//    //myCad->AddCone(25, 0, 50);
//}


void GlfwOcctView::mainloop()
{
    myTools.IniPNG();
    while (!glfwWindowShouldClose(myOcctWindow->getGlfwWindow()))
    {
        glfwPollEvents();

        myController->Flush();

        myGui.BeginFrame();

        const bool changed = myCadPanel.Draw(myHistory, *myDocument);
        myGui.Draw();
        myTools.Draw(myHistory, *myDocument);
        myGui.EndFrame();

        glfwSwapBuffers(myOcctWindow->getGlfwWindow());

        if (changed)
            myContext->UpdateCurrentViewer();
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
