cmake_minimum_required(VERSION 3.9)
project(fractal)

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    message("-- OpenGL found")
    include_directories(${OpenGL_INCLUDE_DIRS})
    link_directories(${OpenGL_LIBRARY_DIRS})
    add_definitions(${OpenGL_DEFINITIONS})
endif(OpenGL_FOUND)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("-- GLEW found")
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
    add_definitions(${GLEW_DEFINITIONS})
endif()

find_package(glfw3 REQUIRED)
IF(glfw3_FOUND)
    message("-- GLFW3 found")
    include_directories(${GLFW_INCLUDE_DIRS})
    link_directories(${GLFW_LIBRARY_DIRS})
    add_definitions(${GLFW_DEFINITIONS})
ENDIF(glfw3_FOUND)

find_package(GLUT REQUIRED)
IF(GLUT_FOUND)
    message("-- GLUT found")
    include_directories(${GLUT_INCLUDE_DIRS})
    link_directories(${GLUT_LIBRARY})
    add_definitions(${GLUT_DEFINITIONS})
ENDIF(GLUT_FOUND)

add_definitions(-std=c++11 -DGLEW_STATIC -lGL -lGLU -lglut)
ADD_EXECUTABLE (
        fractal
        main.cpp
        src/window.cpp src/window.h
        src/load_shaders.cpp src/load_shaders.h
        src/fractal.cpp src/fractal.h
        )

target_link_libraries(
        fractal
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLUT_LIBRARIES})