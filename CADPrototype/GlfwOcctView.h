#ifndef _GlfwOcctView_Header
#define _GlfwOcctView_Header

#include "GlfwOcctWindow.h"
#include "OcctInputController.h"

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

#include <memory>
#include "GuiApp.h"
#include "CadSession.h"



struct GLFWwindow;

// Sample class creating 3D Viewer within GLFW window.
class GlfwOcctView
{
public:
    GlfwOcctView();
    ~GlfwOcctView();

    void run();

private:
    void initWindow(int theWidth, int theHeight, const char* theTitle);
    void initViewer();
    void initGui();
    void renderGui();
    void initDemoScene();
    void initViewCube();
    void mainloop();
    void cleanup();

private:
    // callbacks
    void onResize(int theWidth, int theHeight);
    void onMouseScroll(double theOffsetX, double theOffsetY);
    void onMouseButton(int theButton, int theAction, int theMods);
    void onMouseMove(int thePosX, int thePosY);

private:
    static void errorCallback(int theError, const char* theDescription);
    static GlfwOcctView* toView(GLFWwindow* theWin);

    static void onResizeCallback(GLFWwindow* theWin, int theWidth, int theHeight)
    {
        toView(theWin)->onResize(theWidth, theHeight);
    }
    static void onFBResizeCallback(GLFWwindow* theWin, int theWidth, int theHeight)
    {
        toView(theWin)->onResize(theWidth, theHeight);
    }
    static void onMouseScrollCallback(GLFWwindow* theWin, double theOffsetX, double theOffsetY)
    {
        toView(theWin)->onMouseScroll(theOffsetX, theOffsetY);
    }
    static void onMouseButtonCallback(GLFWwindow* theWin, int theButton, int theAction, int theMods)
    {
        toView(theWin)->onMouseButton(theButton, theAction, theMods);
    }
    static void onMouseMoveCallback(GLFWwindow* theWin, double thePosX, double thePosY)
    {
        toView(theWin)->onMouseMove((int)thePosX, (int)thePosY);
    }

private:
    Handle(GlfwOcctWindow) myOcctWindow;
    Handle(V3d_View) myView;
    Handle(AIS_InteractiveContext) myContext;

    std::unique_ptr<OcctInputController> myController;
    GuiApp myGui;
    std::unique_ptr<CadSession> myCad;
};

#endif // _GlfwOcctView_Header
