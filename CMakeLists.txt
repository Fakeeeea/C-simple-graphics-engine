cmake_minimum_required(VERSION 3.27)
project(tridimensional C)

set(CMAKE_C_STANDARD 11)

add_executable(tridimensional src/main.c
        src/tridimensional_math.h
        src/tridimensional_math.c
        src/windows_math.h
        src/windows_math.c
        src/model.c
        src/model.h
        src/types.c)
