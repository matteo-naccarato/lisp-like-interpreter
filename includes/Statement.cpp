#include "Statement.h"
#include "Visitor.h"

void SetStmt::accept(Visitor* v) {
    v->visitSet(this);
}

void InputStmt::accept(Visitor* v) {
    v->visitInput(this);
}

void PrintStmt::accept(Visitor* v) {
    v->visitPrint(this);
}

void IfStmt::accept(Visitor* v) {
    v->visitIf(this);
}

void WhileStmt::accept(Visitor* v) {
    v->visitWhile(this);
}