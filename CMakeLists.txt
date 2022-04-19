cmake_minimum_required(VERSION 3.22)
project (CalTrain C)

set(CMAKE_C_STANDARD 99)
set(Sources caltrain.c)
set(Headers caltrain.h)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
add_executable(CalTrain caltrain-runner.c ${Sources} ${Headers})
