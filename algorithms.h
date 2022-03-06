#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "circle.h"
#include "cone.h"
#include "cylinder.h"
#include "freehand.h"
#include "line.h"
#include "rect.h"
#include "shape.h"
#include "sphere.h"
#include "strokes.h"
#include <initializer_list>
#include <triangle.h>

class Algorithms {
public:
  Algorithms();
  static Cone *coneDetection(Strokes *pStrokes, GLfloat xRotation,
                             GLfloat yRotation, GLfloat zRotation);
  static Cylinder *cylinderDetection(Strokes *pStrokes, bool isTube,
                                     bool hasBase, GLfloat xRotation,
                                     GLfloat yRotation, GLfloat zRotation);
  static Rect *rectangleDetection(Strokes *pStrokes, GLfloat xRotation,
                                  GLfloat yRotation, GLfloat zRotation);
  static Circle *circleDetection(Strokes *pStrokes, GLfloat xRotation,
                                 GLfloat yRotation, GLfloat zRotation);
  static Triangle *triangleDetection(Strokes *pStrokes, GLfloat xRotation,
                                     GLfloat yRotation, GLfloat zRotation);
  static Sphere *sphereDetection(Strokes *pStrokes, GLfloat xRotation,
                                 GLfloat yRotation, GLfloat zRotation);
  static Freehand *freehandDetection(Strokes *pStrokes, GLfloat xRotation,
                                     GLfloat yRotation, GLfloat zRotation);
  static bool pointAboveLine(Point *head, Point *tail, Point *check);
  static Point *farPoint(Line *line, Point *point);
  static GLfloat degreeToRadian(GLfloat degree);
  static GLfloat radianToDegree(GLfloat rad);
  static Point *averagePoint(Line *pLine);
  static GLfloat distBtwPoints(Point *a, Point *b);
  static Point *maxPerpDistance(Point *head, Point *tail, Line *curve);
  static Point *closestPoint(std::initializer_list<Point*> points);
  static Line *angularTolerance(Line *line, GLfloat tolerance);
  static Line *slideAveraging(Line *line);
  static Line *slideAveraging(Line *line, int iterations);
  static Point *createAveragePoint(std::initializer_list<Point*> points);
  static GLfloat slope(Point *p1, Point *p2);
  static GLfloat distPointToLine(Point *pt, GLfloat slope, GLfloat x1,
                                 GLfloat y1);
  static GLfloat angleBtwLines(GLfloat slope1, GLfloat slope2);
};

#endif // ALGORITHMS_H
