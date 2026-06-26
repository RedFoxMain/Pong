cmake_minimum_required (VERSION 3.8)
project ("Pong")

set(SRC_FILES
	"src/main.cpp"
	"src/ball.cpp"
	"src/paddle.cpp"
	"src/menu.cpp"
	"src/game.cpp"
	"src/state_manager.cpp"
)
set(HEADER_FILES
	"include/ball.hpp"
	"include/paddle.hpp"
	"include/constants.hpp"
	"include/utils.hpp"
	"include/menu.hpp"
	"include/game.hpp"
	"include/game_packet.hpp"
)

set(FONT_DIR "resources/ArialRegular.ttf" CACHE STRING "Path to fonts directory")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/font_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/font_config.h"
    @ONLY
)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/ArialRegular.ttf" DESTINATION "${PROJECT_BINARY_DIR}/resources/")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 3.0.0 COMPONENTS Graphics Network System REQUIRED)
add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Network SFML::System)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)