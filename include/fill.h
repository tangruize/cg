//
// Created by tangruize on 17-11-30.
//

#ifndef CG_FILL_H
#define CG_FILL_H

class FillAlgo {
private:
    static void doFloodFill(int x, int y);

    struct Direction {
        int xOff, yOff;
    };
    static Direction directions[4];
    static int fillSeq;
public:
    static void draw(int x, int y) {
        floodFill(x, y);
    }

    static void floodFill(int x, int y);
};

#endif //CG_FILL_H
