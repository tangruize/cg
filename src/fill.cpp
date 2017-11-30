//
// Created by tangruize on 17-11-30.
//

#include "fill.h"
#include "shape.h"
#include "window.h"
#include "draw.h"

void FillAlgo::floodFill(int x, int y) {
    win.beforeDrawPoint();
    fillSeq = win.getFillSeq();
    win.toggleFillMode();
    doFloodFill(x, y);
    win.toggleFillMode();
    win.afterDrawPoint();
}

void FillAlgo::doFloodFill(int x, int y) {
    int seq = win.readFillSeq(x, y);
    if (seq && seq != fillSeq) {
        win.write(x, y);
        for (int i = 0; i < 4; ++i) {
            doFloodFill(x + directions[i].xOff, y + directions[i].yOff);
        }
    }
}

FillAlgo::Direction FillAlgo::directions[4] = {
        {0,  1},
        {1,  0},
        {0,  -1},
        {-1, 0}
};

int FillAlgo::fillSeq;