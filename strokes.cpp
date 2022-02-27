#include "strokes.h"
#include <iostream>
using namespace std;
Strokes::Strokes(Line * head):
    noOfLines(1), pHeadLine(head), pTailLine(head)
{}

Strokes::~Strokes(){
    delete pHeadLine;
    pHeadLine=0;
    pTailLine=0;
}

void Strokes::addLine(Line * line){
    pTailLine->next=line;
    pTailLine=line;
    noOfLines++;
}
