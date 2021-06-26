cmake_minimum_required(VERSION 3.17)
project(multLang)

set(CMAKE_CXX_STANDARD 14)

add_executable(multLang main.cpp frontend/frontend.cpp frontend/frontend.h tree/tree.h tree/tree.cpp textLib/textLib.h textLib/textLib.cpp
              backend/backend.h backend/backend.cpp)