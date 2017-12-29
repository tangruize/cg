#ifndef CG_LINE_H
#define CG_LINE_H

class LineAlgo {
public:
    static void draw(int x1, int y1, int x2, int y2) {
//        dda(x1, y1, x2, y2);
        BresenhamLine(x1, y1, x2, y2);
    }

    static void dda(int x1, int y1, int x2, int y2);

    static void BresenhamLine(int x1, int y1, int x2, int y2);
};

#endif