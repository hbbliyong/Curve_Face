# GLAD的CMakeLists.txt
cmake_minimum_required(VERSION 3.10)  # 兼容较早的CMake版本[3](@ref)
project(glad)

# 创建GLAD库目标
add_library(glad src/glad.c)  # 根据你的实际路径调整[4,6](@ref)

# 包含头文件目录
target_include_directories(glad PUBLIC include)  # PUBLIC使依赖项目也能找到头文件[4,6](@ref)

# 设置编译器特性（可选但推荐）
target_compile_features(glad PRIVATE c_std_99)  # GLAD需要C99支持

# 设置编译属性（可选）
if(UNIX AND NOT APPLE)
    # 在Linux上可能需要链接libdl
    find_library(DL_LIBRARY dl)
    target_link_libraries(glad PUBLIC ${DL_LIBRARY})
endif()

if(WIN32)
    # 在Windows上可能需要定义某些宏
    target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT)
endif()

# 设置属性，确保符号可见性（重要）
set_target_properties(glad PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)