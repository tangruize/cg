#ifndef CG_FILE_H
#define CG_FILE_H

#include <string>
#include <utility>

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

    explicit File(std::string n) : name(std::move(n)) {}

    File() : File(FILE_NAME) {}

    void setFile(const std::string &n) {
        name = n;
    }

    void save();

    void load(bool noError = false);
};

extern class File file;

#endif