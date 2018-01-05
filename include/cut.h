//
// Created by tangruize on 18-1-3.
//

#ifndef CG_CUT_H
#define CG_CUT_H

#include <utility>
#include <vector>
using namespace std;

typedef pair<int, int> point;

struct Point {
    int x, y, loc;
    Point(const point &p): x(p.first), y(p.second), loc(0) {}
    Point(int px, int py): x(px), y(py), loc(0) {}
    Point() { x = y = loc = 0; }
};

class Cut {
public:
    static void cut(int x1, int y1, int x2, int y2);
    static void polygonCut(vector<Point> &src, vector<point> &result);
    static void lineCut(point &p1, point &p2);
    static bool pointCut(const point &p);
};


#endif //CG_CUT_H
