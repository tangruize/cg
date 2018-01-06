#include "file.h"
#include "shape.h"
#include "window.h"

#include <fstream>
#include <iostream>

using namespace std;

void File::save() {
    ofstream out(name, ios::binary);
    if (!out.is_open()) {
        cerr << "SAVE: cannot open file \"" << name << "\"" << endl;
        return;
    }
    for (auto &shape : Shape::shapes) {
        shape->save(out);
    }
    out.close();
}

void File::load(bool noError) {
    Shape::clearAll();
    ifstream in(name, ios::binary);
    if (!in.is_open()) {
        if (!noError)
            cerr << "LOAD: cannot open file \"" << name << "\"" << endl;
        return;
    }
    Shape *s;
    while ((s = Shape::getShapeFromStream(in)) != NULL) {
        Shape::push(s);
    }
}

class File file;

const char *const File::strFileOp[] = {
        "Save", "Load"
};