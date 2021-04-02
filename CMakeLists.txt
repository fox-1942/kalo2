cmake_minimum_required(VERSION 3.17)
project(kalo)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        src/main.c
        src/camera.c
        src/draw.c
        src/model.c
        src/vector_calc.c)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} glut32 SOIL glu32 opengl32)