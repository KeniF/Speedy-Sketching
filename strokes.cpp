#include "strokes.h"
#include <iostream>
using namespace std;
Strokes::Strokes(Line *head) : noOfLines(1), pHeadLine(head), pTailLine(head) {}

Strokes::~Strokes() {
  delete pHeadLine;
  // line destructor handles deletion of next lines
}

void Strokes::addLine(Line *line) {
  pTailLine->setNext(line);
  pTailLine = line;
  noOfLines++;
}
