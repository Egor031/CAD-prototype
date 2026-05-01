#pragma once
/*
#include <AIS_InteractiveContext.hxx>

#include "Document.h"
#include "History.h"

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

#include "Document.h"

class CreateBox
{
public:
    CreateBox(double nx, double ny, double nz, double len, double wid, double hei );

    void Apply(Document& doc);//override;
    void Undo(Document& doc);//override;

    std::string Name() const /*override*/ { return "CreateBox"; }
    std::string ToJson() const;//override;

private:
    double NullX, NullY, NullZ, Lenght, Width, Height;
    EntityId myCreatedId = 0;
};
