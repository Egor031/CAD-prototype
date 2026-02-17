#pragma once

#include <AIS_ViewController.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

class GlfwOcctWindow;

// Контроллер ввода: получает события GLFW и управляет OCCT View через AIS_ViewController.
class OcctInputController : public AIS_ViewController
{
public:
    OcctInputController(const Handle(V3d_View)& theView,
        const Handle(AIS_InteractiveContext)& theCtx,
        const Handle(GlfwOcctWindow)& theWindow);

    void OnResize(int theWidth, int theHeight);
    void OnMouseScroll(double theOffsetX, double theOffsetY);
    void OnMouseButton(int theButton, int theAction, int theMods);
    void OnMouseMove(int thePosX, int thePosY);

    // flush OCCT events each frame
    void Flush();

    // should we wait events or poll?
    bool ToWaitEvents() const { return myToWaitEvents; }

protected:
    void handleViewRedraw(const Handle(AIS_InteractiveContext)& theCtx,
        const Handle(V3d_View)& theView) override;

private:
    Handle(V3d_View) myView;
    Handle(AIS_InteractiveContext) myContext;
    Handle(GlfwOcctWindow) myWindow;
    bool myToWaitEvents = true;
};
