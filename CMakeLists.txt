cmake_minimum_required(VERSION 3.6)
project(cg)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp
        src/window.cpp include/window.h
        src/coordinate.cpp include/coordinate.h
        src/line.cpp include/line.h
        src/display.cpp include/display.h
        src/draw.cpp include/draw.h
        src/file.cpp include/file.h
        src/menu.cpp include/menu.h
        src/mouse.cpp include/mouse.h
        src/shape.cpp include/shape.h
        src/keyboard.cpp include/keyboard.h
        src/ellipse.cpp include/ellipse.h
        src/color.cpp include/color.h
        src/fill.cpp include/fill.h src/curve.cpp include/curve.h src/cut.cpp include/cut.h)

add_executable(cg ${SOURCE_FILES})
target_link_libraries(cg ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} m)