cmake_minimum_required(VERSION 2.8)

project(MVC)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Includes
include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

# Set some variables
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MVC)
set(SOURCES ${SRC_DIR}/batch.cpp
            ${SRC_DIR}/control.cpp
            ${SRC_DIR}/geometry.cpp
            ${SRC_DIR}/image.cpp
            ${SRC_DIR}/main.cpp
            ${SRC_DIR}/membrane.cpp
            ${SRC_DIR}/patch.cpp
)

# Create an executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(MVC ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
