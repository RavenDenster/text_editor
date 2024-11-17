
#include "rope.hpp"
#include "command_rope.hpp"
#include "command_manager.hpp"

namespace text_editor
{
class TerminalInterface {
public:
    TerminalInterface() : myRope() {} 
   
    void run();

private:
    text_editor::rope myRope;
    text_editor::CommandManager cmdManager;

    void processCommand(const std::string& command);

    void handleInsert(const std::string& params);

    void handleDelete(const std::string& params);

    void handleAppend(const std::string& text);

    void handleFind(const std::string& text);

    void handleSubSting(const std::string& params);
};
}