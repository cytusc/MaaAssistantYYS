cmake_minimum_required(VERSION 3.20)
project(MaaYYS VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MAAYYS_USE_OPENCV "Enable OpenCV-based template matching" OFF)

if(MAAYYS_USE_OPENCV)
    find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)
    message(STATUS "OpenCV found: ${OpenCV_VERSION}")
    message(STATUS "OpenCV libs: ${OpenCV_LIBS}")
endif()

file(GLOB_RECURSE MAAYYS_SRC CONFIGURE_DEPENDS
    "src/MaaYYS/Base/*.cpp"
    "src/MaaYYS/Activity/*.cpp"
    "src/MaaYYS/Daily/*.cpp"
    "src/MaaYYS/Daily/Tasks/*.cpp"
    "src/MaaYYS/Daily/Tasks/Orochi/*.cpp"
    "src/MaaYYS/Common/Controller/*.cpp"
    "src/MaaYYS/Common/Flow/*.cpp"
    "src/MaaYYS/Common/Resource/*.cpp"
    "src/MaaYYS/Common/Vision/*.cpp"
)

add_library(MaaYYS STATIC ${MAAYYS_SRC})

target_include_directories(MaaYYS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MaaYYS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MaaCore
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MaaUtils/include
)

target_compile_features(MaaYYS PUBLIC cxx_std_20)

if(MAAYYS_USE_OPENCV)
    target_compile_definitions(MaaYYS PUBLIC MAAYYS_WITH_OPENCV)
    target_include_directories(MaaYYS PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(MaaYYS PUBLIC ${OpenCV_LIBS})
endif()

# 暂时移除 MaaUtils 依赖，待后续完善构建系统后再添加
# target_link_libraries(MaaYYS PUBLIC MaaUtils)

add_executable(OrochiDryRun src/MaaYYS/Tools/OrochiDryRun.cpp)
target_link_libraries(OrochiDryRun PRIVATE MaaYYS)

add_executable(TestResourceLoader src/MaaYYS/Tools/TestResourceLoader.cpp)
target_link_libraries(TestResourceLoader PRIVATE MaaYYS)

if(MAAYYS_USE_OPENCV)
    add_executable(TemplateMatchVerify src/MaaYYS/Tools/TemplateMatchVerify.cpp)
    target_link_libraries(TemplateMatchVerify PRIVATE MaaYYS)
endif()
