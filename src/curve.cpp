//
// Created by tangruize on 18-1-3.
//

#include "curve.h"
#include "line.h"

#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

typedef pair<int, int> point;
typedef vector<point> points;

static void YangHuiTriangle(vector<unsigned long> &arr) {
    for (int i = 1, sz = (int)arr.size() - 1, count = 2; i < sz; count += ++i & 1) {
        unsigned long pre = 1, ppre;
        for (int j = 1; j < count; ++j) {
            ppre = pre;
            pre = arr[j];
            arr[j] += ppre;
            arr[i - j + 1] = arr[j];
        }
    }
}

static void BezierPoint(double u, point &dst, points &ctrl, vector<unsigned long> &arr) {
    int n = (int)ctrl.size() - 1;
    double bezier, x = 0, y = 0;
    for(int i = 0; i <= n; ++i){
        bezier = arr[i] * pow(u, i) * pow(1 - u, n - i);
        x += ctrl[i].first * bezier;
        y += ctrl[i].second * bezier;
    }
    dst.first = (int)x;
    dst.second = (int)y;
}

static void bezier(points &ctrl, int precision) {
    vector<unsigned long> arr(ctrl.size(), 1);
    YangHuiTriangle(arr);
    points dst((unsigned)precision + 1);
    for(int i = 0; i <= precision; ++i) {
        double u = (double)i / precision;
        BezierPoint(u, dst[i], ctrl, arr);
    }
    for (int i = 0; i < precision; ++i) {
        LineAlgo::draw(dst[i].first, dst[i].second, dst[i + 1].first, dst[i + 1].second);
    }
}

void CurveAlgo::Bezier(points &vertices) {
    bezier(vertices, PRECISION);
}
