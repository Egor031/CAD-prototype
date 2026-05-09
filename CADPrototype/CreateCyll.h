/*
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
*/

#pragma once

#include "ICommand.h"
#include "Document.h"
#include <memory>

class CreateCyll : public ICommand
{
public:
    CreateCyll(double nullx, double nully, double nullz, double dia, double hei, double axx, double axy, double axz);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "CreateCyll"; }
    std::string ToJson() const override;

public:
    double myDx, myDy, myDz, Diametr, Height, AxisX, AxisY, AxisZ;
    EntityId myCreatedId = 0;
};
