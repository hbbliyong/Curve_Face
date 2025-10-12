cmake_minimum_required(VERSION 3.28)
project(Curve_Face LANGUAGES CXX)

# 1. 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 设置资源目录宏
set(RESOURCE_DIR "${PROJECT_SOURCE_DIR}/assets")
set(SHADER_DIR "${RESOURCE_DIR}/shaders")
set(TEXTURE_DIR "${RESOURCE_DIR}/textures")
set(MODEL_DIR "${RESOURCE_DIR}/models")


# 2. 自动调用MOC、UIC等Qt工具
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(QTROOT_DIR C:/Qt/6.9.2)
message(${QTROOT_DIR})
# Qt
set(Qt6_DIR ${QTROOT_DIR}/msvc2022_64/lib/cmake/Qt6)
find_package(Qt6 COMPONENTS 
Core 
Gui 
Widgets 
OpenGLWidgets
Xml
REQUIRED)

# 4. **关键：查找系统的原生OpenGL库**
#find_package(OpenGL REQUIRED)

add_subdirectory(sources)
add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/glm)