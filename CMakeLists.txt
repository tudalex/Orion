project(Orion)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_BUILD_TYPE "Release")
cmake_minimum_required(VERSION 2.6)
add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory(attica)

set(testSRCS main.cpp mainwindow.cpp)
set(testHDRS mainwindow.h)
QT4_WRAP_CPP(test_MOC_SRCS ${testHDRS})
find_package(Qt4 REQUIRED)

include_directories( ${QT_INCLUDES} )

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable(test ${testSRCS} ${test_MOC_SRCS})
target_link_libraries(test ${QT_LIBRARIES} attica)

