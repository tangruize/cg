#ifndef CG_COORDINATE_H
#define CG_COORDINATE_H

#define PI 3.1415926

class Coordinate {
private:
    static int srcX, srcY, srcX2, srcY2, toX, toY;
    static double info, info2;
public:
    static int type;
    static void coorTrans(float x, float y, float &fx, float &fy);

    enum {
        T_MOVE, T_ROTATE, T_ZOOM,
    };

    static const int NR_TRANS = 3;
    static const char *const strTrans[NR_TRANS];

    static void setSrc(int x, int y);

    static void setSrc2(int x, int y);

    static void setDest(int x, int y);

    static void setType(int t);

    static void getTransResult(int &x, int &y);

};

#endif