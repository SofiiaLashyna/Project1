cmake_minimum_required(VERSION 3.29)
project(Project1)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        Entities
        Graph
        Algorithms
        UnitTests
)

file(GLOB SOURCES_MAIN
        "main.cpp"
        "Entities/*.cpp"
        "Graph/*.cpp"
        "Algorithms/*.cpp"
)

file(GLOB SOURCE_TESTS
        "UnitTests/*.cpp"
        "Entities/*.cpp"
        "Graph/*.cpp"
        "Algorithms/*.cpp"
)

set(CMAKE_PREFIX_PATH "D:/Qt/6.9.3/mingw_64")

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)


qt6_wrap_cpp(MOC_SOURCES Graph/GraphWidget.h)

add_executable(Project1
        ${SOURCES_MAIN}
        ${MOC_SOURCES}
)

add_executable(ProjectTests
        ${SOURCE_TESTS}

)

target_link_libraries(Project1 PRIVATE Qt6::Core Qt6::Widgets)

