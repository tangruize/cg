#ifndef CG_DRAW_H
#define CG_DRAW_H

void clearAll();

void addPoint(const struct point &s);

void addThread(const struct thread &s);

void addTriangle(const struct triangle &s);

void addRectangle(const struct rectangle &s);

void addCircle(const struct circle &s);

void drawPoint(const struct point &s);

void drawThread(const struct thread &s);

void drawTriangle(const struct triangle &s);

void drawRectangle(const struct rectangle &s);

void drawCircle(const struct circle &s);

void drawAllPoints();

void drawAlllThreads();

void drawAllTriangles();

void drawAllRectangles();

void drawAllCircles();

void drawAllShapes();

void load();

void save();

#endif