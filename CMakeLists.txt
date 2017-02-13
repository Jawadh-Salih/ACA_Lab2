cmake_minimum_required(VERSION 3.5.0)
project(mytest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        framework.c
        framework.h
        main.c
        predictors.c
        predictors.h
        )

add_executable(BPAnalysis ${SOURCE_FILES})
