cmake_minimum_required(VERSION 3.9)
project(stanford_bunny)

set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

if(OpenGL_FOUND)
    message("-- OpenGL found")
endif(OpenGL_FOUND)

if (GLEW_FOUND)
    message("-- GLEW found")
endif()

IF(glfw3_FOUND)
    message("-- GLFW3 found")
ENDIF(glfw3_FOUND)

IF(GLUT_FOUND)
    message("-- GLUT found")
ENDIF(GLUT_FOUND)

IF (assimp_FOUND)
    message("-- assimp is found")
ENDIF(assimp_FOUND)

include_directories(${assimp_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${OpenGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
        )

link_libraries(${GLEW_LIBRARIES})

link_directories(
        ${OpenGL_LIBRARY_DIRS}
        ${GLFW_LIBRARY_DIRS}
        ${GLUT_LIBRARY}
        ${assimp_LIBRARY_DIRS}
)

add_definitions(
        ${OpenGL_DEFINITIONS}
        ${GLEW_DEFINITIONS}
        ${GLFW_DEFINITIONS}
        ${GLUT_DEFINITIONS}
        ${assimp_DEFINITIONS}
)

add_definitions(-std=c++11 -DGLEW_STATIC -lGL -lGLU -lglut -DGLEW_STATIC)

add_executable(
        ${PROJECT_NAME}
        src/main.cpp

        src/bunny_with_shadows.cpp src/bunny_with_shadows.h

        src/program/window.cpp src/program/window.h
        src/program/camera.cpp src/program/camera.h
        src/program/shaders.cpp src/program/shaders.h

        src/model/model.cpp src/model/model.h
        src/model/mesh.cpp src/model/mesh.h
        src/model/vertex.h
)

target_link_libraries(
        ${PROJECT_NAME}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLUT_LIBRARIES}
        ${ASSIMP_LIBRARIES}
)