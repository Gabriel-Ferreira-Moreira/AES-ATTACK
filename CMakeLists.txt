cmake_minimum_required(VERSION 3.18)

project(AES-Attack C CXX)

set(CMAKE_C_STANDARD 99) # Usando C
set(CMAKE_CXX_STANDARD 17) # Usando C++

add_executable(main
    src/main.c src/aes.c)
target_include_directories(main PRIVATE include)

add_executable(main2
    src/main2.c src/aes.c)
target_include_directories(main2 PRIVATE include)

# Executável de testes (GoogleTest)
add_executable(test
    src/test.cpp src/aes.c)
target_include_directories(test PRIVATE include)

# Linka com GoogleTest 
find_package(GTest REQUIRED)
target_link_libraries(test PRIVATE GTest::GTest GTest::Main pthread)
