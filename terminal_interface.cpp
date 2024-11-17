#include <iostream>
#include <string>
#include "terminal_interface.hpp"

namespace text_editor
{

void TerminalInterface::run() {
    std::string command;
    while (true) {
        std::cout << "Введите команду (exit для выхода): ";
        std::getline(std::cin, command);
        processCommand(command);
    }
}

void TerminalInterface::processCommand(const std::string& command) {
    if (command == "exit") {
        std::cout << "Выход из программы..." << std::endl;
        std::exit(0);
    } else if (command.rfind("insert ", 0) == 0) {
        handleInsert(command.substr(7));
    } else if (command.rfind("delete ", 0) == 0) {
        handleDelete(command.substr(7));
    } else if (command.rfind("append ", 0) == 0) {
        handleAppend(command.substr(7));
    } else if (command.rfind("find ", 0) == 0) {
        handleFind(command.substr(5));
    } else if (command.rfind("substring ", 0) == 0) {
        handleSubSting(command.substr(10));
    } else if (command == "show") {
        std::cout << "Текущий текст: " << myRope.toString() << std::endl;
    } else if (command == "undo") {
        cmdManager.undo(myRope);
    } else if (command == "redo") {
        cmdManager.redo(myRope);
    } else if (command == "history") {
        cmdManager.printCommandHistory();
    } else if (command == "look_rope") {
        myRope.print_tree(std::cout);
    }  else if (command == "length_text") {
        std::cout << "Длина текущего текста: " << myRope.length() << std::endl;
    } else {
        std::cout << "Неизвестная команда." << std::endl;
    }
}

void TerminalInterface::handleInsert(const std::string& params) {
    size_t index = std::stoi(params.substr(0, params.find(' ')));
    std::string text = params.substr(params.find(' ') + 1);
    text_editor::InsertCommand* insertCmd = new text_editor::InsertCommand(index, text);
    cmdManager.executeCommand(insertCmd, myRope);
    std::cout << "Вставлено: " << text << std::endl;
    if(!myRope.isBalanced()) myRope.balance();
}

void TerminalInterface::handleDelete(const std::string& params) {
    size_t index = std::stoi(params.substr(0, params.find(' ')));
    size_t length = std::stoi(params.substr(params.find(' ') + 1));
    text_editor::DeleteCommand* deleteCmd = new text_editor::DeleteCommand(index, length, myRope.substring(index, length));
    cmdManager.executeCommand(deleteCmd, myRope);
    std::cout << "Удалено " << length << " символов с позиции " << index << std::endl;
    if(!myRope.isBalanced()) myRope.balance();
}

void TerminalInterface::handleAppend(const std::string& text) {
    text_editor::AppendCommand* appendCmd = new text_editor::AppendCommand(text);
    cmdManager.executeCommand(appendCmd, myRope);
    std::cout << "Добавлено в конец: " << text << std::endl;
    if(!myRope.isBalanced()) myRope.balance();
}

void TerminalInterface::handleFind(const std::string& text) {
    std::string full_text = myRope.toString();
    auto pos = full_text.find(text);
    if (pos != std::string::npos) {
        std::cout << "Найдено на позиции: " << pos << std::endl;
    } else {
        std::cout << "Не найдено." << std::endl;
    }
}

void TerminalInterface::handleSubSting(const std::string& params) {
    size_t spacePos = params.find(' ');
    size_t index = std::stoi(params.substr(0, spacePos));
    size_t len_string = std::stoi(params.substr(spacePos + 1));

    std::string sub_string = myRope.substring(index, len_string);
    std::cout << "Подстрока: " << sub_string << std::endl;
}
}