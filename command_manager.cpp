#include <iostream>
#include "command_manager.hpp"

namespace text_editor
{
    void CommandManager::executeCommand(Command* cmd, rope& r) {
        // Если current_ указывает на старую команду, удаляем все последующие команды
        if (current_ + 1 < history_.size()) {
            history_.erase(history_.begin() + current_ + 1, history_.end());
        }

        // Выполняем команду
        cmd->execute(r);
        
        // Превышает ли размер истории 25?
        if (history_.size() >= 25) {
            // Удаляем самую старую команду
            history_.erase(history_.begin());
            current_--; // Сдвигаем текущую команду на -1, чтобы учесть удаление
        }
        
        // Сохраняем новую команду в истории
        history_.push_back(std::unique_ptr<Command>(cmd));
        current_ = history_.size() - 1; // Устанавливаем текущую команду
    }
    void CommandManager::undo(rope& r) {
        if (current_ >= 0) {
            history_[current_]->undo(r);
            current_--;
        }
    }

    void CommandManager::redo(rope& r) {
        // Проверяем, можем ли произвести redo
        if (current_ + 1 < history_.size()) {
            current_++;
            history_[current_]->execute(r);
        }
    }

    void CommandManager::printCommandHistory() {
        std::cout << "История команд: ";
        for (size_t i = 0; i < history_.size(); ++i) {
            if (i == current_) {
                std::cout << "-> "; // Используйте стрелку, чтобы указать текущую команду
            }
            std::cout << "Команда " << i + 1 << (i == history_.size()-1 ? "" : ", ");
        }
        std::cout << std::endl;
    }
}