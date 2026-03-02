cmake_minimum_required(VERSION 3.16)

# Name the Test Project
project(tst_evtol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically handle Qt Meta-Object Compiler (MOC)
set(CMAKE_AUTOMOC ON)

# Find the Qt Test Module
find_package(Qt6 REQUIRED COMPONENTS Test)

# Define the Source Logic Library
# (We compile the vehicle logic here so the test can link to it)
add_library(VehicleLogic 
    ../evtolvehicle.cpp
    ../evtolvehicle.h
)
target_include_directories(VehicleLogic PUBLIC ..)

# Define the Test Executable
add_executable(tst_evtol
    tst_evtoltests.cpp
)

# Link the Test Executable to:
# 1. The Vehicle Logic
# 2. The Qt Test Framework
target_link_libraries(tst_evtol PRIVATE 
    VehicleLogic 
    Qt6::Test
)