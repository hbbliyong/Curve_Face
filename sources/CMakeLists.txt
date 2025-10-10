set(LIBRARY_NAME Curve_Face)

file(GLOB_RECURSE HEADER_FILES "include/*.h*")
file(GLOB_RECURSE SOURCE_FILES "src/*.c*")
add_executable(${LIBRARY_NAME}
   ${SOURCE_FILES}
   ${HEADER_FILES}
)

# 6. 链接库
# 先链接Qt6 Widgets，再链接系统的OpenGL库
target_link_libraries(${PROJECT_NAME} 
Qt6::Widgets
 Qt6::OpenGLWidgets
 glad
#  ${OPENGL_LIBRARIES}
)

# 7. 包含头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE include)