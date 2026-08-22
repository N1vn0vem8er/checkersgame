cmake_minimum_required(VERSION 3.16)

project(checkersgame LANGUAGES CXX)

find_package(raylib REQUIRED raylib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(checkersgame src/main.cpp
    src/checkersai.h src/checkersai.cpp

    src/board.h src/board.cpp
    src/state.h
    src/menu.h src/menu.cpp)

target_link_libraries(checkersgame PRIVATE raylib)

include(GNUInstallDirs)
install(TARGETS checkersgame
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
