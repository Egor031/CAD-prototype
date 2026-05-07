/*#pragma once

#include <AIS_InteractiveContext.hxx>

#include "Document.h"

struct GLFWwindow;

class CreateBox
{
public:
    CreateBox() = default;
    ~CreateBox() = default;

    std::string DrawBox(char InX[100], char  InY[100], char  InZ[100], char Len[100], char  Wid[100], char  Hei[100], Document& doc);

private:
    bool myInitialized = false;
};
*/

#pragma once

#include "ICommand.h"
#include "Document.h"
#include <memory>

class CreateBox : public ICommand
{
public:
    CreateBox(double nullx, double nully, double nullz, double len, double wid, double hei);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "CreateBox"; }
    std::string ToJson() const override;

public:
    double myDx, myDy, myDz, Lenght, Width, Height;
    EntityId myCreatedId = 0;
};
