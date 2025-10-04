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
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

# --- Añadir CPR automáticamente ---
include(FetchContent)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG master
)
FetchContent_MakeAvailable(cpr)

# --- Archivos fuente ---
set(SOURCES
    main.cpp
    src/axios.cpp
)

add_executable(climapp ${SOURCES})

# --- Incluir y enlazar librerías ---
target_include_directories(climapp PRIVATE 
    ${GDAL_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(climapp PRIVATE 
    ${QT_LIBS} 
    ${GDAL_LIBRARIES}
    ${CURL_LIBRARIES}
    cpr::cpr
    nlohmann_json::nlohmann_json
)

# --- Copiar recursos al directorio de compilación ---
file(COPY resources/polygons.txt DESTINATION ${CMAKE_BINARY_DIR}/resources)
file(COPY resources/styles.scss DESTINATION ${CMAKE_BINARY_DIR}/resources)

# --- Propiedades Qt ---
set_target_properties(climapp PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    OUTPUT_NAME "climapp"
)

# --- Forzar extensión .exe incluso en Linux ---
set_target_properties(climapp PROPERTIES SUFFIX ".exe")
