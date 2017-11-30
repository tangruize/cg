//
// Created by tangruize on 17-11-29.
//

#ifndef CG_ELLIPSE_H
#define CG_ELLIPSE_H

class Oval {
protected:
    Oval() {}

    static int tmp_x, tmp_y;

    static void doDraw(float x, float y, bool isCicle = false);
};

class CircleAlgo : private Oval {
private:
    CircleAlgo() {}

public:
    static void BresenhamCircle(int xc, int yc, float r);

    static void draw(int xc, int yc, float r) {
        BresenhamCircle(xc, yc, r);
    }
};

class EllipseAlgo : private Oval {
private:
    EllipseAlgo() {}

public:
    static void BresenhamEllipse(int mx, int my, int a, int b);

    static void draw(int mx, int my, int a, int b) {
        BresenhamEllipse(mx, my, a, b);
    }
};

#endif //CG_ELLIPSE_H
