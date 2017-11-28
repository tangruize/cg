#include "file.h"
#include "shape.h"
#include "draw.h"

#include <GL/glut.h>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

const char *file_name = FILE_NAME;
static int _null_shape[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
static char *null_shape = reinterpret_cast<char *>(_null_shape);
static char tmp_shape[8];

void doSave(vector<point> &points,
            vector<thread> &threads,
            vector<triangle> &triangles,
            vector<rectangle> &rectangles,
            vector<circle> &circles) {
    ofstream out(file_name, ios::binary);
    if (!out.is_open()) {
        cerr << "save '" << file_name << "' failed\n";
        return;
    }
    for (unsigned i = 0; i < points.size(); ++i)
        out.write(reinterpret_cast<char *>(&points[i]), sizeof(points[i]));
    out.write(null_shape, sizeof(struct point));
    for (unsigned i = 0; i < threads.size(); ++i)
        out.write(reinterpret_cast<char *>(&threads[i]), sizeof(threads[i]));
    out.write(null_shape, sizeof(struct thread));
    for (unsigned i = 0; i < triangles.size(); ++i)
        out.write(reinterpret_cast<char *>(&triangles[i]), sizeof(triangles[i]));
    out.write(null_shape, sizeof(struct triangle));
    for (unsigned i = 0; i < rectangles.size(); ++i)
        out.write(reinterpret_cast<char *>(&rectangles[i]), sizeof(rectangles[i]));
    out.write(null_shape, sizeof(struct rectangle));
    for (unsigned i = 0; i < circles.size(); ++i)
        out.write(reinterpret_cast<char *>(&circles[i]), sizeof(circles[i]));
    out.write(null_shape, sizeof(struct circle));
    out.close();
}

void doLoad(vector<point> &points,
            vector<thread> &threads,
            vector<triangle> &triangles,
            vector<rectangle> &rectangles,
            vector<circle> &circles) {
    ifstream in(file_name, ios::binary | ios::ate);
    if (!in.is_open()) {
        cerr << "load '" << file_name << "' failed\n";
        return;
    }
    unsigned size = (unsigned) in.tellg();
    in.seekg(0);
    unsigned i = 0;
    for (; i < size; i += sizeof(struct point)) {
        in.read(tmp_shape, sizeof(struct point));
        if (reinterpret_cast<struct point *>(tmp_shape)->color == -1)
            break;
        points.push_back(*reinterpret_cast<struct point *>(tmp_shape));
    }
    for (; i < size; i += sizeof(struct thread)) {
        in.read(tmp_shape, sizeof(struct thread));
        if (reinterpret_cast<struct thread *>(tmp_shape)->color == -1)
            break;
        threads.push_back(*reinterpret_cast<struct thread *>(tmp_shape));
    }
    for (; i < size; i += sizeof(struct triangle)) {
        in.read(tmp_shape, sizeof(struct triangle));
        if (reinterpret_cast<struct triangle *>(tmp_shape)->color == -1)
            break;
        triangles.push_back(*reinterpret_cast<struct triangle *>(tmp_shape));
    }
    for (; i < size; i += sizeof(struct rectangle)) {
        in.read(tmp_shape, sizeof(struct rectangle));
        if (reinterpret_cast<struct rectangle *>(tmp_shape)->color == -1)
            break;
        rectangles.push_back(*reinterpret_cast<struct rectangle *>(tmp_shape));
    }
    for (; i < size; i += sizeof(struct circle)) {
        in.read(tmp_shape, sizeof(struct circle));
        if (reinterpret_cast<struct circle *>(tmp_shape)->color == -1)
            break;
        circles.push_back(*reinterpret_cast<struct circle *>(tmp_shape));
    }
    in.close();
    drawAllShapes();
    glFlush();
}