cmake_minimum_required(VERSION 3.17)
project(kalo)

add_compile_options(-std=gnu99)

add_executable(${PROJECT_NAME}
        src/controller.c
        src/camera.c
        src/draw.c
        src/basic_primitives.c
        src/main.c
        src/load_model.c
        src/model.c
        )

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} glut32 SOIL glu32 opengl32)