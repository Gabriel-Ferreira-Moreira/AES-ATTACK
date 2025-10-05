cmake_minimum_required(VERSION 3.18)

project(AES-Attack C CXX)

set(CMAKE_C_STANDARD 99) # Usando C
set(CMAKE_CXX_STANDARD 17) # Usando C++

add_executable(main1
    src/attack1.c src/aes.c)
target_include_directories(main1 PRIVATE include)

add_executable(main2
    src/attack2.c src/aes.c)
target_include_directories(main2 PRIVATE include)

add_executable(main3
    src/attack3.c src/aes.c)
target_include_directories(main3 PRIVATE include)

add_executable(main4
    src/attack4.c src/aes.c)
target_include_directories(main4 PRIVATE include)

# Executável de testes (GoogleTest)
add_executable(test
    src/test.cpp src/aes.c)
target_include_directories(test PRIVATE include)

# Linka com GoogleTest 
find_package(GTest REQUIRED)
target_link_libraries(test PRIVATE GTest::GTest GTest::Main pthread)
