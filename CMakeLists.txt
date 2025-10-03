cmake_minimum_required(VERSION 3.16)
project(climapp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar Qt6 primero, si no usar Qt5
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
    set(QT_LIBS Qt5::Widgets Qt5::OpenGL)
else()
    set(QT_LIBS Qt6::Widgets Qt6::OpenGLWidgets)
endif()

find_package(GDAL REQUIRED)

set(SOURCES
    main.cpp
)

add_executable(climapp ${SOURCES})

target_include_directories(climapp PRIVATE ${GDAL_INCLUDE_DIRS})
target_link_libraries(climapp PRIVATE ${QT_LIBS} ${GDAL_LIBRARIES})

# Copia de archivos de datos y estilos al build
file(COPY data/polygons.txt DESTINATION ${CMAKE_BINARY_DIR}/data)
file(COPY styles.scss DESTINATION ${CMAKE_BINARY_DIR})

set_target_properties(climapp PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
