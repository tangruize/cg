//
// Created by tangruize on 17-11-30.
//

#include "fill.h"
#include "shape.h"
#include "window.h"
#include "draw.h"

#include <stack>

using namespace std;

void FillAlgo::floodFill(int x, int y) {
    win.beforeDrawPoint();
    fillSeq = win.getFillSeq();
    win.toggleFillMode();
    doFloodFill(x, y);
    win.toggleFillMode();
    win.afterDrawPoint();
}

void FillAlgo::doFloodFill(int x, int y) {
//    if (!xp || xp->empty()) {
        int seq, x1, y1;
        stack<int> xd, yd;
        xd.push(x);
        yd.push(y);
        while (!xd.empty()) {
            x1 = xd.top();
            y1 = yd.top();
            xd.pop();
            yd.pop();
            seq = win.readFillSeq(x1, y1);
            if (seq && seq != fillSeq) {
                win.write(x1, y1);
                if (xp) {
                    xp->push_back(x1);
                    yp->push_back(y1);
                }
                for (int i = 0; i < 4; ++i) {
//                doFloodFill(x + directions[i].xOff, y + directions[i].yOff);
                    xd.push(x1 + directions[i].xOff);
                    yd.push(y1 + directions[i].yOff);
                }
            }
        }
//    } else {
//        for (int i = 0; i < xp->size(); ++i) {
//            win.write((*xp)[i], (*yp)[i]);
//        }
//    }
    xp = yp = NULL;
}

FillAlgo::Direction FillAlgo::directions[4] = {
        {0,  1},
        {1,  0},
        {0,  -1},
        {-1, 0}
};

void FillAlgo::setFillVertice(std::vector<int> &x, std::vector<int> &y) {
    xp = &x;
    yp = &y;
}

int FillAlgo::fillSeq;
vector<int> *FillAlgo::xp;
vector<int> *FillAlgo::yp;