#pragma once

#include <AIS_InteractiveContext.hxx>

#include "Document.h"

struct GLFWwindow;

class CreateBox
{
public:
    CreateBox() = default;
    ~CreateBox() = default;

    void DrawBox(char InX[100], char  InY[100], char  InZ[100], char Len[100], char  Wid[100], char  Hei[100] );

private:
    bool myInitialized = false;
    Document Document;
};
