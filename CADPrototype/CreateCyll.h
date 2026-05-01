#pragma once

#include <AIS_InteractiveContext.hxx>

#include "Document.h"

struct GLFWwindow;

class CreateCyll
{
public:
    CreateCyll() = default;
    ~CreateCyll() = default;

    std::string DrawCyll(char InX[100], char  InY[100], char  InZ[100], char Diam[100], char  Hei[100], char  axelX[100], char  axelY[100], char  axelZ[100], Document& doc);

private:
    bool myInitialized = false;
};
