cmake_minimum_required(VERSION 3.18)

project(AES-Attack C)

set(CMAKE_C_STANDARD 99)

add_executable(main
    src/main.c src/aes.c)
target_include_directories(main PRIVATE include)

add_executable(main2
    src/main2.c src/aes.c)
target_include_directories(main2 PRIVATE include)


#add_executable(Enc-Dec
#   src/Enc-Dec.c src/aes.c src/convert.c)
#target_include_directories(Enc-Dec PRIVATE include)
