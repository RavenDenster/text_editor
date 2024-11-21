#pragma once
#include <iostream>
#include "rope.hpp"

namespace text_editor
{
class InsertCommand : public Command {
public:
    InsertCommand(size_t idx, const std::string& str) : index_(idx), str_(str) {}
    
    void execute(rope& r) override;

    void undo(rope& r) override { r.rdelete(index_, str_.length());}

private:
    size_t index_;
    std::string str_;
};

class DeleteCommand : public Command {
public:
    DeleteCommand(size_t idx, size_t len, const std::string& str) : index_(idx), length_(len), str_(str) {}

    void execute(rope& r) override;

    void undo(rope& r) override { r.insert(index_, str_); }

private:
    size_t index_;
    size_t length_;
    std::string str_;
};

class AppendCommand : public Command {
public:
    AppendCommand(const std::string& str) : str_(str) {}

    void execute(rope& r) override { r.append(str_);}

    void undo(rope& r) override;

private:
    std::string str_;
};
}