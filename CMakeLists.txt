cmake_minimum_required(VERSION 3.6)
project(cg)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp include/window.h
        src/circle.cpp include/circle.h
        src/coordinate.cpp include/coordinate.h
        src/dda.cpp include/dda.h
        src/display.cpp include/display.h
        src/draw.cpp include/draw.h
        src/file.cpp include/file.h
        src/menu.cpp include/menu.h
        src/mouse.cpp include/mouse.h
        src/shape.cpp include/shape.h)

add_executable(cg ${SOURCE_FILES})
target_link_libraries(cg ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})