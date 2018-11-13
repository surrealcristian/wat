#-----------------------------------
# cmake version
#-----------------------------------
cmake_minimum_required (VERSION 3.4)

#-----------------------------------
# project name
#-----------------------------------
project (wat)

#-----------------------------------
# packages
#-----------------------------------
find_package(SDL2 REQUIRED COMPONENTS SDL2::SDL2)

#-----------------------------------
# own vars
#-----------------------------------
# add flags to compiler
add_compile_options(-Wall -O3 -I/usr/include/SDL2 -D_REENTRANT)


#-----------------------------------
# sources
#-----------------------------------
add_executable (${PROJECT_NAME}
 ${CMAKE_SOURCE_DIR}/src/bullet.c
 ${CMAKE_SOURCE_DIR}/src/bullet_manager.c
 ${CMAKE_SOURCE_DIR}/src/collision_manager.c
 ${CMAKE_SOURCE_DIR}/src/config.c
 ${CMAKE_SOURCE_DIR}/src/enemy.c
 ${CMAKE_SOURCE_DIR}/src/enemy_manager.c
 ${CMAKE_SOURCE_DIR}/src/game.c
 ${CMAKE_SOURCE_DIR}/src/input_component.c
 ${CMAKE_SOURCE_DIR}/src/in_game_state.c
 ${CMAKE_SOURCE_DIR}/src/particle.c
 ${CMAKE_SOURCE_DIR}/src/particle_manager.c
 ${CMAKE_SOURCE_DIR}/src/pause_state.c
 ${CMAKE_SOURCE_DIR}/src/player.c
 ${CMAKE_SOURCE_DIR}/src/text.c
 ${CMAKE_SOURCE_DIR}/src/tinymt32.c
 ${CMAKE_SOURCE_DIR}/src/util.c
 ${CMAKE_SOURCE_DIR}/src/wat.c
)


#-----------------------------------
# include headers
#-----------------------------------
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_include_directories (${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})


#-----------------------------------
# link libraries
#-----------------------------------
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
