#ifndef CG_FILE_H
#define CG_FILE_H

#include "shape.h"

#include <vector>

#define FILE_NAME "cg_saved.data"
extern const char *file_name;

void doSave(std::vector<point> &points,
            std::vector<thread> &threads,
            std::vector<triangle> &triangles,
            std::vector<rectangle> &rectangles,
            std::vector<circle> &circles);

void doLoad(std::vector<point> &points,
            std::vector<thread> &threads,
            std::vector<triangle> &triangles,
            std::vector<rectangle> &rectangles,
            std::vector<circle> &circles);

#endif