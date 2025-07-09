cmake_minimum_required(VERSION 3.15)

project(cse576-2020sp-HW2)

set(CMAKE_CXX_FLAGS "-Wall -fdiagnostics-color=always -std=c++11 -pthread -O3 -g -march=native -fPIC")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(uwimg++ SHARED
     src/utils.cpp
     src/utils.h
     src/image.h
     src/load_image.cpp
     src/stb_image.h
     src/stb_image_write.h
     
     src/process_image.cpp
     
     src/resize_image.cpp  # file to submit
     src/filter_image.cpp  # file to submit
     
     
     )
 
link_libraries(uwimg++ m stdc++)

add_compile_options(-Wno-int-in-bool-context)
add_compile_options(-Wint-in-bool-context)

add_executable(test0 src/test/test0.cpp)
add_executable(test1 src/test/test1.cpp)


