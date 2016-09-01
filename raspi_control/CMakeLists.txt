#cmake version
cmake_minimum_required(VERSION 2.8)
#Project Name
PROJECT(LineFollower)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/)
# find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)
link_directories(/opt/vc/lib)

#header file
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
#library path
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

#compiler option
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -fPIC -static-libstdc++")
#debug option
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -ggdb")
#release option
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src ALL_SRC)
add_executable(LineFollower ${ALL_SRC})
# add_executable(LineFollower src/main.cpp)

#link libraries
target_link_libraries(LineFollower SioClientWrapper)
target_link_libraries(LineFollower sioclient)
target_link_libraries (LineFollower ${OpenCV_LIBS})
# target_link_libraries (LineFollower ${raspicam_CV_LIBS})
#Add pthread
find_package(Threads REQUIRED)
target_link_libraries(LineFollower "${CMAKE_THREAD_LIBS_INIT}")

# make static library
# add_library(SioClientWrapper STATIC src/SioClientWrapper.cpp)
