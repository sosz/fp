cmake_minimum_required(VERSION 3.6)
project(fp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(WARNING_FLAGS "-Wall -Werror -Wno-variadic-macros -fmax-errors=1 -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")

include(cmake/glob_sources.cmake)
include(cmake/package.cmake)
include(cmake/boost_package.cmake)

add_definitions(-DBOOST_PP_VARIADICS)
boost_package(preprocessor)
#boost_package(mpl DEPENDENCIES preprocessor)
#boost_package(iterator DEPENDENCIES core detail static_assert)
#boost_package(range DEPENDENCIES
#    config assert concept_check utility type_traits mpl iterator optional
#    throw_exception move
#)
finalize_boost_packages(boost-1.64.0)

include_directories(src)

add_subdirectory(src/fp)
add_subdirectory(test)
add_subdirectory(profile)
