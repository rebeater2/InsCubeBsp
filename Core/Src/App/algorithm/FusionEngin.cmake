cmake_minimum_required(VERSION 3.19)
set(MAHONY_6_AXIS "6-axis mahony")
set(MAHONY_9_AXIS "9-axis mahony")

option(MAHONY "Set to build with mahony algorithm" OFF)

if(DEFINED FUSION_ENGIN)
    set(CMAKE_FUSION_ENGIN ${FUSION_ENGIN})
    message("The fusion algorithm is ${CMAKE_FUSION_ENGIN}")
endif()


if(CMAKE_FUSION_ENGIN EQUAL ${MAHONY_6_AXIS})
    message("Configure for ${CMAKE_FUSION_ENGIN}")
    add_compile_options(-DMAHONY_6_AXIS)
    include_directories(Mahony)
elseif(CMAKE_FUSION_ENGIN EQUAL ${MAHONY_6_AXIS})
    message(STATUS "${CMAKE_FUSION_ENGIN} is not supported")
else()
    message(STATUS "Unknown algorithm ${CMAKE_FUSION_ENGIN}")
endif()

