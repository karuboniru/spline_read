cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(xmlread)

set(cxx_standard 14)

find_package(Boost REQUIRED)
find_package(ROOT REQUIRED)
include("${ROOT_USE_FILE}")

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${BOOST_INCLUDE_DIRS}
                    ${ROOT_INCLUDE_DIRS})

#add_library(xmlreader src/xmlreader.cc)
add_library(splineread SHARED src/splineread.cc)

add_executable(demo demo.cc)
target_link_libraries(demo ${ROOT_LIBRARIES} splineread)