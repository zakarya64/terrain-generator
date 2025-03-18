cmake_minimum_required(VERSION 3.10)
project(TerrainRenderer)

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGLWidgets)

add_executable(TerrainRenderer main.cpp terrainwidget.cpp terrain.cpp)
target_link_libraries(TerrainRenderer Qt6::Core Qt6::Gui Qt6::OpenGLWidgets)
