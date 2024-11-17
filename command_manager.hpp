#pragma once
#include "rope.hpp"

namespace text_editor
{
class CommandManager {
public:
    void executeCommand(Command* cmd, rope& r);
    void undo(rope& r);
    void redo(rope& r);
    void printCommandHistory();
    
private:
    std::vector<std::unique_ptr<Command>> history_;
    int current_ = -1; // Устанавливаем текущую команду на -1
};
}