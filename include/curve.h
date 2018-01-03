//
// Created by tangruize on 18-1-3.
//

#ifndef CG_CURVE_H
#define CG_CURVE_H

#include <vector>
#include <utility>

class CurveAlgo {
private:
    static const int PRECISION = 200;
public:
    static void Bezier(std::vector<std::pair<int, int>> &vertices);
    static void draw(std::vector<std::pair<int, int>> &vertices) {
        Bezier(vertices);
    }
};


#endif //CG_CURVE_H
