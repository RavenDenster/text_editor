#include "command_rope.hpp"

namespace text_editor {
    std::invalid_argument ERROR_NODE = std::invalid_argument("Error: string index out of bounds");

    void InsertCommand::execute(rope& r) {
        if (index_ > r.length()) {
            throw ERROR_NODE;
        }
        r.insert(index_, str_);
    }
    
    void DeleteCommand::execute(rope& r) {
        if (index_ > r.length() || index_ + length_ > r.length()) {
            throw ERROR_NODE;
        }
        r.rdelete(index_, length_);
    }

    void AppendCommand::undo(rope& r) {
        size_t str_len = str_.length();
        size_t current_length = r.length();
        r.rdelete(current_length - str_len, str_len);
    }

}