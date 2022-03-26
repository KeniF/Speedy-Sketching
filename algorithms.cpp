#include "algorithms.h"
#include "cmath"
#include <iostream>
#define isnan(x) ((x) != (x))

static const GLfloat PI = 3.141592653589793238462f;

Cone *Algorithms::coneDetection(Strokes *pStrokes, GLfloat xRotation,
                                GLfloat yRotation, GLfloat zRotation) {
  Triangle *triangle =
      triangleDetection(pStrokes, xRotation, yRotation, zRotation);
  GLfloat angle1 = 0.0f, angle2 = 0.0f, angle3 = 0.0f;
  GLfloat slope12 = slope(triangle->getP1(), triangle->getP2()),
          slope23 = slope(triangle->getP2(), triangle->getP3()),
          slope31 = slope(triangle->getP3(), triangle->getP1());
  angle2 = angleBtwLines(slope12, slope23);
  angle3 = angleBtwLines(slope23, slope31);
  angle1 = angleBtwLines(slope31, slope12);
  GLfloat diff12 = abs(angle1 - angle2), diff23 = abs(angle2 - angle3),
          diff31 = abs(angle3 - angle1);
  Point *base1 = 0, *base2 = 0, *baseMid = 0, *centre = 0, *oldTop = 0,
        *newTop = 0;
  GLfloat height = 0.0f, tilt = 0.0f;
  if (diff12 < diff23 && diff12 < diff31) { // 12
    base1 = triangle->getP1();
    base2 = triangle->getP2();
    oldTop = triangle->getP3();
  } else if (diff23 < diff12 && diff23 < diff31) { // 23
    base1 = triangle->getP2();
    base2 = triangle->getP3();
    oldTop = triangle->getP1();
  } else if (diff31 < diff12 && diff31 < diff23) { // 31
    base1 = triangle->getP3();
    base2 = triangle->getP1();
    oldTop = triangle->getP2();
  } else {
    cout << "equilateral!?!? WTF!!" << endl;
    delete triangle;
    return 0;
  }
  baseMid = createAveragePoint({base1, base2}); // delete
  tilt = slope(base1, base2);
  GLfloat tiltAngle = radianToDegree(atan(tilt));
  cout << tiltAngle << endl;
  height = distPointToLine(oldTop, tilt, base2->getX(), base2->getY());
  if (!pointAboveLine(base1, base2, oldTop))
    tiltAngle += 180.0f;
  centre = createAveragePoint({base1, base2});
  delete baseMid;
  delete newTop;
  delete triangle;
  return new Cone(centre, distBtwPoints(base1, base2), height, xRotation,
                  yRotation, tiltAngle);
}

Cylinder *Algorithms::cylinderDetection(Strokes *pStrokes, bool isTube,
                                        bool hasBase, GLfloat xRotation,
                                        GLfloat yRotation, GLfloat zRotation) {
  Rect *rect = rectangleDetection(pStrokes, xRotation, yRotation, zRotation);
  Point *newCentre = new Point(*rect->getCentre());
  Cylinder *cy = new Cylinder(isTube, hasBase, newCentre, rect->getWidth(),
                              rect->getLength(), rect->getXRotation(),
                              rect->getYRotation(), rect->getZRotation());
  delete rect;
  return cy;
}

Rect *Algorithms::rectangleDetection(Strokes *pStrokes, GLfloat xRotation,
                                     GLfloat yRotation, GLfloat zRotation) {
  Point *t1 = 0, *t2 = 0, *td1 = 0, *td2 = 0;
  if (pStrokes->getTotalLines() > 4 || pStrokes->getTotalLines() <= 1)
    return 0;
  else if (pStrokes->getTotalLines() == 2) {
    Line *line1 = pStrokes->getHeadLine();
    Line *line2 = line1->getNext();
    GLfloat slope1 = slope(line1->getHeadPoint(), line1->getTail());
    GLfloat slope2 = slope(line2->getHeadPoint(), line2->getTail());
    GLfloat totalAngle1 = 0.0f;
    GLfloat totalAngle2 = 0.0f;
    GLfloat tempSlope = 0.0f;
    Point *iterator = line1->getHeadPoint();
    int noOfPoints1 = 0, noOfPoints2 = 0;
    while ((iterator = iterator->getNext())) {
      tempSlope = slope(line1->getHeadPoint(), iterator);
      if (!isinf(tempSlope) && !isnan(tempSlope))
        totalAngle1 += angleBtwLines(slope1, tempSlope);
      noOfPoints1++;
    }
    iterator = line2->getHeadPoint();
    while ((iterator = iterator->getNext())) {
      tempSlope = slope(line2->getHeadPoint(), iterator);
      if (!isinf(tempSlope) && !isnan(tempSlope))
        totalAngle2 += angleBtwLines(slope2, tempSlope);
      noOfPoints2++;
    }
    GLfloat avgAngle1 = totalAngle1 / (GLfloat)noOfPoints1;
    GLfloat avgAngle2 = totalAngle2 / (GLfloat)noOfPoints2;
    cout << "ang " << avgAngle1 << " " << avgAngle2 << endl;
    if (avgAngle1 / 5.0f > avgAngle2 || avgAngle2 / 5.0f > avgAngle1) { // [ |
      Line *cLine = 0;
      Line *lLine = 0;
      // line1 ], line2 |
      if (avgAngle1 / 5.0f > avgAngle2) {
        cLine = line1;
        lLine = line2;
      }
      // line1 |, line2 ]
      else {
        cLine = line2;
        lLine = line1;
      }
      Point *cHead = cLine->getHeadPoint();
      // cHead--lHead
      if (distBtwPoints(cHead, lLine->getHeadPoint()) <
          distBtwPoints(cHead, lLine->getTail())) {
        t1 = createAveragePoint({cHead, lLine->getHeadPoint()});
        t2 = createAveragePoint({cLine->getTail(), lLine->getTail()});
      }
      // cHead--lTail
      else {
        t2 = createAveragePoint({cLine->getTail(), lLine->getHeadPoint()});
        t1 = createAveragePoint({cHead, lLine->getTail()});
      }
      td1 = farPoint(cLine, t1); // diagonal from t1
      td2 = farPoint(cLine, t2); // diagonal from t2
    }
    // line1 _| line2 _| 2Ls
    else {
      // head-head
      if (distBtwPoints(line1->getHeadPoint(), line2->getHeadPoint()) <
          distBtwPoints(line1->getHeadPoint(), line2->getTail())) {
        t1 = createAveragePoint({line1->getHeadPoint(), line2->getHeadPoint()});
        td1 = createAveragePoint({line1->getTail(), line2->getTail()});
        t2 = maxPerpDistance(line1->getHeadPoint(), line1->getTail(), line1);
        td2 = maxPerpDistance(line2->getHeadPoint(), line2->getTail(), line2);
      }
      // Head--Tail
      else {
        t1 = createAveragePoint({line1->getHeadPoint(), line2->getTail()});
        td1 = createAveragePoint({line1->getTail(), line2->getHeadPoint()});
        t2 = maxPerpDistance(line1->getHeadPoint(), line1->getTail(), line1);
        td2 = maxPerpDistance(line2->getHeadPoint(), line2->getTail(), line2);
      }
    }
  } else if (pStrokes->getTotalLines() == 3) {
    return 0;
  } else if (pStrokes->getTotalLines() == 4) {
    return 0;
  }
  // formed 4 random points in space!
  // cout<<t1->toString()<<" "<<t2->toString()<<" "<<td1->toString()<<"
  // "<<td2->toString()<<endl;
  GLfloat avgWidth, avgLength;
  GLfloat avgLength1 = (distBtwPoints(td2, t1) + distBtwPoints(td1, t2)) / 2.0f;
  GLfloat avgLength2 = (distBtwPoints(td2, td1) + distBtwPoints(t1, t2)) / 2.0f;
  // cout<<avgLength1<<"length"<<avgLength2<<endl;
  GLfloat slope_t1td2, slope_t2td1, slope_td1td2, slope_t1t2;
  GLfloat totalSlope = 0.0f;
  int noOfSlopes = 0;
  if (avgLength1 > avgLength2) {
    avgLength = avgLength1;
    avgWidth = avgLength2;
    slope_t1td2 = slope(td2, t1);
    slope_t2td1 = slope(td1, t2);
    // slope_td1td2=-1.0f/slope(td1,td2);
    // slope_t1t2=-1.0f/slope(t1,t2);
    if (!isinf(slope_t1td2) && !isnan(slope_t1td2)) {
      totalSlope += slope_t1td2;
      noOfSlopes++;
    }
    if (!isinf(slope_t2td1) && !isnan(slope_t2td1)) {
      totalSlope += slope_t2td1;
      noOfSlopes++;
    }
  } else {
    avgLength = avgLength2;
    avgWidth = avgLength1;
    // slope_t1td2=-1.0f/slope(td2,t1);
    // slope_t2td1=-1.0f/slope(td1,t2);
    slope_td1td2 = slope(td1, td2);
    slope_t1t2 = slope(t1, t2);
    if (!isinf(slope_td1td2) && !isnan(slope_td1td2)) {
      totalSlope += slope_td1td2;
      noOfSlopes++;
    }
    if (!isinf(slope_t1t2) && !isnan(slope_t1t2)) {
      totalSlope += slope_t1t2;
      noOfSlopes++;
    }
  }
  // cout<<"w"<<avgWidth<<"l"<<avgLength<<endl;
  // cout<<"Slopes "<<slope_t1td2<<" "<<slope_t2td1<<" "<<slope_td1td2<<"
  // "<<slope_t1t2<<endl;
  if (noOfSlopes == 0)
    return 0; // no slope is valid
  GLfloat avgSlope = totalSlope / (GLfloat)noOfSlopes;
  GLfloat angleFromNeutral = radianToDegree(atan(abs(avgSlope)));
  // cout<<avgSlope<<" "<<angleFromNeutral<<endl;
  if (avgSlope < 0.0f)
    angleFromNeutral = 180.0f - angleFromNeutral;
  Point *centre = createAveragePoint({t1, t2, td1, td2});
  // delete t1;
  // delete t2;
  // delete td1;
  // delete td2; //MEMORY LEAKAGE?
  return new Rect(centre, avgWidth, avgLength, xRotation, yRotation,
                  angleFromNeutral);
}

Freehand *Algorithms::freehandDetection(Strokes *pStrokes, GLfloat xRotation,
                                        GLfloat yRotation, GLfloat zRotation) {
  Line *original = pStrokes->getHeadLine();
  Line *line = angularTolerance(slideAveraging(original), 1.5f);
  return new Freehand(false, line, xRotation, yRotation, zRotation);
}

Circle *Algorithms::circleDetection(Strokes *pStrokes, GLfloat xRotation,
                                    GLfloat yRotation, GLfloat zRotation) {
  // Point * pAvg=averagePoint(pLine);
  Line *pLine = pStrokes->getHeadLine();
  GLfloat total = 0.0f;
  Point *pAvg = new Point((pLine->getMinX() + pLine->getMaxX()) / 2.0f,
                          (pLine->getMinY() + pLine->getMaxY()) / 2.0f,
                          pLine->getHeadPoint()->getZ());
  Point *pTempPt = pLine->getHeadPoint();
  do
    total += distBtwPoints(pTempPt, pAvg);
  while ((pTempPt = pTempPt->getNext()));
  GLfloat avgRadius = total / (GLfloat)pLine->getTotalPoints();
  return new Circle(pAvg, avgRadius, xRotation, yRotation, zRotation);
}

Sphere *Algorithms::sphereDetection(Strokes *pStrokes, GLfloat xRotation,
                                    GLfloat yRotation, GLfloat zRotation) {
  Line *pLine = pStrokes->getHeadLine();
  GLfloat total = 0.0f;
  Point *pAvg = new Point((pLine->getMinX() + pLine->getMaxX()) / 2.0f,
                          (pLine->getMinY() + pLine->getMaxY()) / 2.0f,
                          pLine->getHeadPoint()->getZ());
  Point *pTempPt = pLine->getHeadPoint();
  do
    total += distBtwPoints(pTempPt, pAvg);
  while ((pTempPt = pTempPt->getNext()));
  GLfloat avgRadius = total / (GLfloat)pLine->getTotalPoints();
  return new Sphere(pAvg, avgRadius, xRotation, yRotation, zRotation);
}

GLfloat Algorithms::angleBtwLines(GLfloat slope1, GLfloat slope2) {
  GLfloat angle;
  if ((isinf(slope1) && isinf(slope2)) || slope1 == slope2) {
    angle = 0.0f; // both vertical/ collinear, do nothing
  } else if (isinf(slope1)) {
    angle = 90.0f - radianToDegree(atan(slope2));
  } else if (isinf(slope2)) {
    angle = 90.0f - radianToDegree(atan(slope1));
  } else {
    GLfloat difference = abs(slope1 - slope2);
    // cout<<slope1<<" "<<slope2<<" "<<difference<<endl;
    angle = abs(radianToDegree(atan(difference / (1.0f + slope1 * slope2))));
  }
  return angle;
}

Triangle *Algorithms::triangleDetection(Strokes *pStrokes, GLfloat xRotation,
                                        GLfloat yRotation, GLfloat zRotation) {
  if (pStrokes->getTotalLines() == 1) {
    Line *line = pStrokes->getHeadLine();
    Point *p1 = createAveragePoint({line->getHeadPoint(), line->getTail()});
    Point *iterator = line->getHeadPoint();
    Point *farPoint = iterator;
    GLfloat maxDistance = 0.0f, temp;
    while ((iterator = iterator->getNext())) {
      if (maxDistance < (temp = distBtwPoints(iterator, p1))) {
        maxDistance = temp;
        farPoint = iterator;
      }
    }
    Point *p2 = new Point(*farPoint);
    GLfloat slopeOfLine = slope(p1, p2);
    GLfloat x1 = p2->getX();
    GLfloat y1 = p2->getY();
    maxDistance = 0.0f;
    iterator = line->getHeadPoint();
    while ((iterator = iterator->getNext())) {
      if (maxDistance <
          (temp = distPointToLine(iterator, slopeOfLine, x1, y1))) {
        farPoint = iterator;
        maxDistance = temp;
      }
    }
    return new Triangle(p1, p2, new Point(*farPoint), xRotation, yRotation,
                        zRotation);
  } else if (pStrokes->getTotalLines() == 2) {
    Line *line1 = pStrokes->getHeadLine();
    Line *line2 = line1->getNext();
    Point *pt1 = line1->getHeadPoint();
    Point *pt2 = line2->getHeadPoint();
    Point *mp1 = createAveragePoint({pt1, line1->getTail()});
    Point *mp2 = createAveragePoint({pt2, line2->getTail()});
    GLfloat slope1 = slope(pt1, line1->getTail());
    GLfloat slope2 = slope(pt2, line2->getTail());
    GLfloat totalAngle1 = 0.0f, totalAngle2 = 0.0f, tempSlope;
    while ((pt1 = pt1->getNext())) {
      tempSlope = slope(mp1, pt1);
      totalAngle1 += angleBtwLines(slope1, tempSlope);
    }
    delete mp1;
    while ((pt2 = pt2->getNext())) {
      tempSlope = slope(mp2, pt2);
      totalAngle2 += angleBtwLines(slope2, tempSlope);
    }
    delete mp2;
    totalAngle1 /= line1->getTotalPoints();
    totalAngle2 /= line2->getTotalPoints();
    GLfloat maxDistance = 0.0f;
    Point *farPoint = 0;
    Point *iterator = 0;
    Line *vLine = 0;
    Line *otherLine = 0;
    if (totalAngle1 > totalAngle2) { // big avg angle from mid-point
      otherLine = line2;
      vLine = line1;
      iterator = vLine->getHeadPoint(); // line1 is > shape;line2 |
    } else {
      otherLine = line1;
      vLine = line2;
      iterator = vLine->getHeadPoint(); // line2 is > shape;line1 |
    }
    farPoint = iterator;
    GLfloat slopeOfOther =
        slope(otherLine->getHeadPoint(), otherLine->getTail());
    GLfloat x1 = otherLine->getTail()->getX();
    GLfloat y1 = otherLine->getTail()->getY();
    GLfloat temp = 0.0f;
    do {
      if (maxDistance <
          (temp = distPointToLine(iterator, slopeOfOther, x1, y1))) {
        farPoint = iterator;
        maxDistance = temp;
      }
    } while ((iterator = iterator->getNext()));
    Point *p3 =closestPoint({vLine->getHeadPoint(), otherLine->getHeadPoint(),
                             otherLine->getTail()});
    Point *p13 = 0, *p24 = 0;
    if (p3 == otherLine->getHeadPoint()) {
      p13 = createAveragePoint({vLine->getHeadPoint(), otherLine->getHeadPoint()});
      p24 = createAveragePoint({vLine->getTail(), otherLine->getTail()});
    } else {
      p13 = createAveragePoint({vLine->getHeadPoint(), otherLine->getTail()});
      p24 = createAveragePoint({vLine->getTail(), otherLine->getHeadPoint()});
    }
    Point *p1 = new Point(*farPoint);
    return new Triangle(p13, p1, p24, xRotation, yRotation, zRotation);
  } else if (pStrokes->getTotalLines() ==
             3) { // Line A(1,2) Line B(3,4), Line C(5,6)
    Line *lineA = pStrokes->getHeadLine();
    Line *lineB = lineA->getNext();
    Line *lineC = lineB->getNext();
    // p1(A)-p6(C)
    Point *p6 =
       closestPoint({lineA->getHeadPoint(), lineB->getHeadPoint(),
                     lineB->getTail(), lineC->getHeadPoint(), lineC->getTail()});
    Point *p16 = createAveragePoint({lineA->getHeadPoint(), p6});
    Point *p3 =
       closestPoint({lineA->getTail(), lineB->getHeadPoint(), lineB->getTail(),
                     lineC->getHeadPoint(), lineC->getTail()});
    Point *p23 = createAveragePoint({lineA->getTail(), p3});
    Point *p4, *p5;
    // ABC layout
    if (p6 == lineC->getHeadPoint() || p6 == lineC->getTail()) {
      if (p6 == lineC->getHeadPoint())
        p5 = lineC->getTail();
      else
        p5 = lineC->getHeadPoint();
      if (p3 == lineB->getHeadPoint())
        p4 = lineB->getTail();
      else
        p4 = lineB->getHeadPoint();
    } else { // ACB layout
      if (p6 == lineB->getHeadPoint())
        p5 = lineB->getTail();
      else
        p5 = lineB->getHeadPoint();
      if (p3 == lineC->getHeadPoint())
        p4 = lineC->getTail();
      else
        p4 = lineC->getHeadPoint();
    }
    Point *p45 = createAveragePoint({p4, p5});

    return new Triangle(p16, p23, p45, xRotation, yRotation, zRotation);
  } else
    return 0;
}

bool Algorithms::pointAboveLine(Point *head, Point *tail, Point *check) {
  GLfloat slopeOfLine = slope(head, tail);
  if (isinf(slopeOfLine)) {
    return check->getX() - head->getX() > 0;
  } else if (slopeOfLine != 0) {
    GLfloat c = head->getY() - head->getX() * slopeOfLine;
    return check->getY() - slopeOfLine * check->getX() - c > 0;
  } else { // horizonal line
    return check->getY() - head->getY() > 0;
  }
}

Point *Algorithms::farPoint(Line *line, Point *point) {
  Point *iterator = 0, *farPoint = 0;
  GLfloat maxDistance = 0.0f;
  GLfloat temp = 0.0f;
  iterator = line->getHeadPoint();
  farPoint = iterator;
  do {
    if (maxDistance < (temp = distBtwPoints(iterator, point))) {
      farPoint = iterator;
      maxDistance = temp;
    }
  } while ((iterator = iterator->getNext()));
  return farPoint;
}

Point *Algorithms::maxPerpDistance(Point *head, Point *tail, Line *curve) {
  GLfloat slp = slope(head, tail);
  Point *iterator, *farPoint = 0;
  GLfloat maxDistance = 0.0f, temp;
  iterator = curve->getHeadPoint();

  if (isinf(slp)) { // vertical line
    do {
      if (maxDistance < abs(temp = (head->getX() - iterator->getX()))) {
        farPoint = iterator;
        maxDistance = temp;
      }
    } while ((iterator = iterator->getNext()));
  } else {
    do {
      if (maxDistance <
          (temp = distPointToLine(iterator, slp, tail->getX(), tail->getY()))) {
        farPoint = iterator;
        maxDistance = temp;
        // cout<<"max dis"<<maxDistance<<endl;
      }
      // cout<<"distance "<<temp<<endl;
    } while ((iterator = iterator->getNext()));
  }
  return farPoint;
} // maxPerpDistance

// Perpendicular distance from point to line, 0=slope*(x-x1)+y1-y
GLfloat Algorithms::distPointToLine(Point *pt, GLfloat slope, GLfloat x1,
                                    GLfloat y1) {
  if (isinf(slope))
    return abs(pt->getX() - x1);
  else {
    GLfloat top = abs(slope * (pt->getX() - x1) + y1 - (pt->getY()));
    return top / sqrt(pow(slope, 2) + 1.0f);
  }
}

Point *Algorithms::averagePoint(Line *pLine) {
  GLfloat sumX = 0.0f, sumY = 0.0f;
  Point *temp = pLine->getHeadPoint();
  do {
    sumX += temp->getX();
    sumY += temp->getY();
  } while ((temp = temp->getNext()) != 0);
  return new Point(sumX / (GLfloat)pLine->getTotalPoints(),
                   sumY / (GLfloat)pLine->getTotalPoints(), 0.0);
}

// Closest point to the first in the initializer_list
Point *Algorithms::closestPoint(std::initializer_list<Point*> points) {
  if (points.size() < 3) throw std::invalid_argument( "Must be at least 3 points to compare");
  auto minDist = std::numeric_limits<float>::infinity();
  Point * closestPoint = 0;
  auto first = std::begin(points);
  Point * firstPoint = *first;
  for (auto it = first + 1; it != std::end(points); ++it) {
    Point *p = *it;
    auto dist = distBtwPoints(firstPoint, p);
    if (minDist > dist) {
      closestPoint = p;
      minDist = dist;
    }
  }
  return closestPoint;
}

GLfloat Algorithms::distBtwPoints(Point *a, Point *b) {
  return hypot((a->getX() - b->getX()), (a->getY() - b->getY()));
}

Line *Algorithms::slideAveraging(Line *line, int iterations) {
  Line *oldLine = line, *newLine = 0;
  bool runOnce = false;
  while (iterations--) {
    newLine = slideAveraging(oldLine);
    if (runOnce)
      delete oldLine;
    oldLine = newLine;
    runOnce = true;
  }
  return newLine;
}

// McMaster 1992
Line *Algorithms::slideAveraging(Line *line) {
  if (line->getTotalPoints() < 5)
    return new Line(*line);
  Point *p1, *p2, *p3, *p4, *p5;
  p1 = line->getHeadPoint();
  p2 = p1->getNext();
  p3 = p2->getNext();
  p4 = p3->getNext();
  p5 = p4->getNext();
  Point *p1copy = new Point(*p1);
  Point *p2copy = new Point(*p2);
  Line *result = new Line(p1copy, line->getXRotation(), line->getYRotation(),
                          line->getZRotation());
  result->addPoint(p2copy);
  do {
    result->addPoint(createAveragePoint({p1, p2, p3, p4, p5}));
    p1 = p2;
    p2 = p3;
    p3 = p4;
    p4 = p5;
  } while ((p5 = p5->getNext()));
  result->addPoint(new Point(*p3));
  result->addPoint(new Point(*(p3->getNext())));
  return result;
}

Point *Algorithms::createAveragePoint(std::initializer_list<Point*> points) {
  float totalX = 0, totalY = 0, totalZ = 0;
  for (auto point : points) {
    totalX += point->getX();
    totalY += point->getY();
    totalZ += point->getZ();
  }
  auto size = points.size();
  return new Point(totalX / size, totalY / size, totalZ / size);
}

// Angular tolerance algorithm (McMaster 1992)
Line *Algorithms::angularTolerance(Line *line, GLfloat tolerance) {
  if (line->getTotalPoints() < 3)
    return new Line(*line);
  else {
    cout << "Points:" << line->getTotalPoints() << "/";
    GLfloat angle;
    Line *final = new Line(
        new Point(line->getHeadPoint()->getX(), line->getHeadPoint()->getY(),
                  line->getHeadPoint()->getZ()),
        line->getXRotation(), line->getYRotation(), line->getZRotation());
    Point *p1 = line->getHeadPoint();
    Point *p2 = p1->getNext();
    Point *p3 = p2->getNext();
    do {
      GLfloat slope1 = slope(p1, p2);
      GLfloat slope2 = slope(p1, p3);
      GLfloat difference;
      if ((isinf(slope1) && isinf(slope2)) || slope1 == slope2) {
        angle = tolerance - 1.0f; // both vertical/ collinear, do nothing
      } else if (isinf(slope1)) {
        angle = 90.0f - radianToDegree(atan(slope2));
      } else if (isinf(slope2)) {
        angle = 90.0f - radianToDegree(atan(slope1));
      } else {
        difference = abs(slope2 - slope1);
        angle =
            abs(radianToDegree(atan(difference / (1.0f + slope1 * slope2))));
      }
      if (angle > tolerance)
        final->addPoint(new Point(*p2)); // keep p2
      p1 = p1->getNext();
      p2 = p2->getNext();
      p3 = p3->getNext();
    } while (p3); // has next
    final->addPoint(new Point(*p2));
    cout << final->getTotalPoints() << endl;
    return final;
  } //>=3 points
}

GLfloat Algorithms::slope(Point *p1, Point *p2) {
  return (p1->getY() - p2->getY()) / (p1->getX() - p2->getX());
}

GLfloat Algorithms::degreeToRadian(GLfloat degree) {
  return degree * PI / 180.0f;
}

GLfloat Algorithms::radianToDegree(GLfloat rad) { return rad * 180.0f / PI; }
