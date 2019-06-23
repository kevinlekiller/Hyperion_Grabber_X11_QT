cmake_minimum_required(VERSION 3.0.0)

project(Hyperion_Grabber_X11_QT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_BUILD_TYPE Desktop)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(X11 REQUIRED)

set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lXrender -lXdamage")
include_directories(${X11_INCLUDE_DIR})
add_executable(${PROJECT_NAME} "main.cpp" "hgx11.h" "hgx11.cpp" "hgx11net.h" "hgx11net.cpp" "hgx11damage.h" "hgx11damage.cpp" "hgx11grab.h" "hgx11grab.cpp")
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Widgets ${X11_LIBRARIES})
