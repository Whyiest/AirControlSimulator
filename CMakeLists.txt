cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME controleaerien-groupe_2_g)
project(${PROJECT_NAME})

set(IMPLEMENTATIONS_FILES
        IMPLEMENTATIONS/Core.cpp
        IMPLEMENTATIONS/FlightsPlanning.cpp
        IMPLEMENTATIONS/Cell.cpp
        IMPLEMENTATIONS/Airport.cpp
        IMPLEMENTATIONS/Aircraft.cpp
        IMPLEMENTATIONS/Network.cpp
        IMPLEMENTATIONS/util.cpp
        IMPLEMENTATIONS/Disruption.cpp
        )
set(HEADERS_FILES
        HEADERS/Libraries.h
        HEADERS/Core.h
        HEADERS/FlightsPlanning.h
        HEADERS/Cell.h
        HEADERS/Airport.h
        HEADERS/Aircraft.h
        HEADERS/Network.h
        HEADERS/util.h
        HEADERS/Disruption.h
        )

add_executable(${PROJECT_NAME} main.cpp "${IMPLEMENTATIONS_FILES}" "${HEADERS_FILES}")
target_include_directories(${PROJECT_NAME} PRIVATE ${IMPLEMENTATIONS_FILES} ${HEADERS_FILES})

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
