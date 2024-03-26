cmake_minimum_required(VERSION 3.7)
project(pong LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/glfw)
add_subdirectory(lib/cglm)

include_directories(lib/glad/include)
include_directories(lib/stb)

list(APPEND CFLAGS -std=c2x)
list(APPEND CFLAGS -Wall)
list(APPEND CFLAGS -Wextra)
list(APPEND CFLAGS -Wstrict-aliasing)
list(APPEND CFLAGS -Wpointer-arith)
list(APPEND CFLAGS -g3)

list(APPEND GLAD lib/glad/src/glad.c)
file(GLOB SRC src/*.c)

add_executable(${PROJECT_NAME} ${GLAD} ${SRC})
target_compile_options(${PROJECT_NAME} PUBLIC ${CFLAGS})

if(WIN32)
    message(NOTICE "Windows has no sound for now")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw cglm openal)
elseif(UNIX) 
    message(NOTICE "Using nas library for loading wav file")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw cglm openal audio)
endif(WIN32)

message(NOTICE "Please run the executable in root directory ./build/pong or ./bin/pong")
