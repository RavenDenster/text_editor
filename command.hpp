#pragma once
#include <iostream>
#include "rope.hpp"

namespace text_editor
{
class rope;
class Command {
public:
    virtual void execute(rope& r) = 0;
    virtual void undo(rope& r) = 0;
    virtual ~Command() {}
};
}