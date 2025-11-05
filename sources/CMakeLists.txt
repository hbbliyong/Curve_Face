set(LIBRARY_NAME Curve_Face)

file(GLOB_RECURSE HEADER_FILES "include/*.h*")
file(GLOB_RECURSE SOURCE_FILES "src/*.c*")

source_group(
   TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
PREFIX "Header Files" 
FILES ${HEADER_FILES})
source_group(
   TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files" 
    FILES ${SOURCE_FILES})

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
 glm::glm
#  ${OPENGL_LIBRARIES}
)

# 7. 包含头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE include)

#方案1： 传递资源路径到源代码
target_compile_definitions(${PROJECT_NAME} PRIVATE
    RESOURCES_PATH="${RESOURCE_DIR}"
    SHADERS_PATH="${SHADER_DIR}"
)
#方案2：拷贝使用相对路径
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCE_DIR}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying resource files"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${PROJECT_NAME}  PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug"
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(${PROJECT_NAME}  PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release"
    )
endif()