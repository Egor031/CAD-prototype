#pragma once

#include "ICommand.h"
#include "Document.h"
#include <memory>

class CreateCut : public ICommand
{
public:
    CreateCut(int id1, int id2);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "CreateCut"; }
    std::string ToJson() const override;

public:
    int Fid1, Fid2;
    EntityId myCreatedId = 0;
};