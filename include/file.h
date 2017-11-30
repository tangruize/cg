#ifndef CG_FILE_H
#define CG_FILE_H

#include <string>

#define FILE_NAME "cg_saved.data"

class File {
private:
    std::string name;
public:
    static const int NR_FILE_OP = 2;
    static const char *const strFileOp[NR_FILE_OP];
    enum FILE_OP {
        SAVE, LOAD
    };

    File(const std::string &n) : name(n) {}

    File() : File(FILE_NAME) {}

    void setFile(const std::string &n) {
        name = n;
    }

    void save();

    void load();
};

extern class File file;

#endif