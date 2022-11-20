cmake_minimum_required(VERSION 3.19)
project(Maximum_Stable_Set_Problem C)

set(CMAKE_C_STANDARD 99)

project(testing)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})

#Get every cpp file in the current directory
file(GLOB CPPFILES "src/*.c")
#for each cpp file found...
foreach(file ${CPPFILES})
    #extract just the file name and extension
    get_filename_component(fname ${file} NAME)
    message(STATUS "Adding target for ${fname}")
    #add target for just that file
    add_executable(run_${fname} src/${fname} src/MiscellaneousFunctions.h src/MiscellaneousFunctions.c)
endforeach()
