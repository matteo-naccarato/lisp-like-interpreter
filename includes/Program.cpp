#include "Program.h"
#include "Visitor.h"

void Program::accept(Visitor* v) {
    v->visitProgram(this);
}