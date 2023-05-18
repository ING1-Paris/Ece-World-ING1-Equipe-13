cmake_minimum_required(VERSION 3.10)
project(PROJET C)

set(CMAKE_C_STANDARD 99)

add_executable(PROJET main.c "Mini-Jeu TANK_TROUBLE.c" Header.h "Mini-Jeu Crossy Road.c" "Mini-Jeu Tir-Aux-Ballons.c" "Mini Jeu peche aux canards.c")
target_link_libraries(PROJET -lmingw32 -lalleg44)
