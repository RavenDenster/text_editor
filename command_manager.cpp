#include <iostream>
#include "command_manager.hpp"

namespace text_editor
{
    void CommandManager::executeCommand(Command* cmd, rope& r) {
        if (current_ + 1 < history_.size()) {
            history_.erase(history_.begin() + current_ + 1, history_.end());
        }
        cmd->execute(r);

        if (history_.size() >= 25) {
            history_.erase(history_.begin());
            current_--;
        }
        
        history_.push_back(std::unique_ptr<Command>(cmd));
        current_ = history_.size() - 1;
    }
    void CommandManager::undo(rope& r) {
        if (current_ >= 0) {
            history_[current_]->undo(r);
            current_--;
        }
    }

    void CommandManager::redo(rope& r) {
        if (current_ + 1 < history_.size()) {
            current_++;
            history_[current_]->execute(r);
        }
    }

    void CommandManager::printCommandHistory() {
        std::cout << "История команд: ";
        for (size_t i = 0; i < history_.size(); ++i) {
            if (i == current_) {
                std::cout << "-> ";
            }
            std::cout << "Команда " << i + 1 << (i == history_.size()-1 ? "" : ", ");
        }
        std::cout << std::endl;
    }
}