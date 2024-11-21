
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
    rope myRope;
    CommandManager cmdManager;

    void processCommand(const std::string& command);

    void handleInsert(const std::string& params);

    void handleDelete(const std::string& params);

    void handleAppend(const std::string& text);

    void handleFind(const std::string& text);

    void handleSubSting(const std::string& params);
};
}