cmake_minimum_required(VERSION 3.16)
project(climapp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Buscar Qt6 primero, luego Qt5 ---
find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)
if(Qt6_FOUND)
    message(STATUS "✅ Usando Qt6")
    set(QT_LIBS Qt6::Widgets Qt6::Charts)
    include_directories(${Qt6Widgets_INCLUDE_DIRS} ${Qt6Charts_INCLUDE_DIRS})
else()
    message(STATUS "⚠️ Usando Qt5")
    find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)
    set(QT_LIBS Qt5::Widgets Qt5::Charts)
    include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Charts_INCLUDE_DIRS})
endif()

# --- Otras librerías ---
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

# --- Crear ejecutable ---
add_executable(climapp ${SOURCES})

# --- Incluir librerías ---
target_include_directories(climapp PRIVATE 
    ${GDAL_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${Qt6Charts_INCLUDE_DIRS} # ✅ asegúrate de incluir QtCharts
)

# --- Enlazar librerías ---
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

# --- Propiedades de Qt ---
set_target_properties(climapp PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    OUTPUT_NAME "climapp"
)

# --- Forzar extensión .exe incluso en Linux (opcional) ---
set_target_properties(climapp PROPERTIES SUFFIX ".exe")
