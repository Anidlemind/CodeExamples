#pragma once
#include <algorithm>
#include <string>

enum class UnaryOperationType {
    INC, DEC
};

enum class BinaryOperationType {
    EQUALS,
    DIFFER,
    SUM,
    SUB,
    MUL,
    DIV,
    MOD,
    MAX,
    MIN,
    AND,
    XOR,
    OR,
    CONC
};

enum class LoopType {
    TILL, WHILE
};

enum class Type {
    NUMBR,
    TROOF,
    YARN
};

std::string bool_to_string(const bool&);