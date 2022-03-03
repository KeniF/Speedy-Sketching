#ifndef STROKES_H
#define STROKES_H
#include <line.h>
class Strokes {
public:
  Strokes(Line *head);
  ~Strokes();
  int getTotalLines() { return noOfLines; }
  Line *getHeadLine() { return pHeadLine; }
  Line *getTailLine() { return pTailLine; }
  void addLine(Line *line);

private:
  int noOfLines;
  Line *pHeadLine;
  Line *pTailLine;
};

#endif // STROKES_H
