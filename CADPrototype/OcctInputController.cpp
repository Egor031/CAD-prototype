#include "OcctInputController.h"
#include "GlfwOcctWindow.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

namespace
{
    static Aspect_VKeyMouse mouseButtonFromGlfw(int theButton)
    {
        switch (theButton)
        {
        case GLFW_MOUSE_BUTTON_LEFT:   return Aspect_VKeyMouse_LeftButton;
        case GLFW_MOUSE_BUTTON_RIGHT:  return Aspect_VKeyMouse_RightButton;
        case GLFW_MOUSE_BUTTON_MIDDLE: return Aspect_VKeyMouse_MiddleButton;
        }
        return Aspect_VKeyMouse_NONE;
    }

    static Aspect_VKeyFlags keyFlagsFromGlfw(int theFlags)
    {
        Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
        if ((theFlags & GLFW_MOD_SHIFT) != 0)   aFlags |= Aspect_VKeyFlags_SHIFT;
        if ((theFlags & GLFW_MOD_CONTROL) != 0) aFlags |= Aspect_VKeyFlags_CTRL;
        if ((theFlags & GLFW_MOD_ALT) != 0)     aFlags |= Aspect_VKeyFlags_ALT;
        if ((theFlags & GLFW_MOD_SUPER) != 0)   aFlags |= Aspect_VKeyFlags_META;
        return aFlags;
    }
}

OcctInputController::OcctInputController(const Handle(V3d_View)& theView,
    const Handle(AIS_InteractiveContext)& theCtx,
    const Handle(GlfwOcctWindow)& theWindow)
    : myView(theView),
    myContext(theCtx),
    myWindow(theWindow)
{
}

void OcctInputController::handleViewRedraw(const Handle(AIS_InteractiveContext)& theCtx,
    const Handle(V3d_View)& theView)
{
    AIS_ViewController::handleViewRedraw(theCtx, theView);
    myToWaitEvents = !myToAskNextFrame;
}

void OcctInputController::OnResize(int theWidth, int theHeight)
{
    if (theWidth != 0 && theHeight != 0 && !myView.IsNull())
    {
        myView->Window()->DoResize();
        myView->MustBeResized();
        myView->Invalidate();
        FlushViewEvents(myContext, myView, true);
    }
}

void OcctInputController::OnMouseScroll(double /*theOffsetX*/, double theOffsetY)
{
    ImGuiIO& aIO = ImGui::GetIO();
    if (!myView.IsNull() && !aIO.WantCaptureMouse)
    {
        UpdateZoom(Aspect_ScrollDelta(myWindow->CursorPosition(), int(theOffsetY * 8.0)));
    }
}

void OcctInputController::OnMouseButton(int theButton, int theAction, int theMods)
{
    ImGuiIO& aIO = ImGui::GetIO();
    if (myView.IsNull() || aIO.WantCaptureMouse)
    {
        return;
    }

    const Graphic3d_Vec2i aPos = myWindow->CursorPosition();
    if (theAction == GLFW_PRESS)
    {
        PressMouseButton(aPos, mouseButtonFromGlfw(theButton), keyFlagsFromGlfw(theMods), false);
    }
    else
    {
        ReleaseMouseButton(aPos, mouseButtonFromGlfw(theButton), keyFlagsFromGlfw(theMods), false);
    }
}

void OcctInputController::OnMouseMove(int thePosX, int thePosY)
{
    if (myView.IsNull())
    {
        return;
    }

    ImGuiIO& aIO = ImGui::GetIO();
    if (!aIO.WantCaptureMouse)
    {
        const Graphic3d_Vec2i aNewPos(thePosX, thePosY);
        UpdateMousePosition(aNewPos, PressedMouseButtons(), LastMouseFlags(), Standard_False);
    }
}

void OcctInputController::Flush()
{
    if (myView.IsNull())
    {
        return;
    }
    myView->InvalidateImmediate(); // redraw even if not modified
    FlushViewEvents(myContext, myView, Standard_True);
}
