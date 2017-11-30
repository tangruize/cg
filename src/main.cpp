#include "window.h"
#include "file.h"

int main(int argc, char *argv[]) {
    if (argc > 1)
        file.setFile(argv[1]);

    win.init(argc, argv);

    return 0;
}