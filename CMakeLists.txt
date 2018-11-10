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
# set flags variables
set (compile_flags "-Wall -O3 -I/usr/include/SDL2 -D_REENTRANT")

# add flags to compiler
set (CMAKE_C_FLAGS     "${compile_flags}")
set (INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
set (SRC_DIR    ${CMAKE_SOURCE_DIR}/src)


#-----------------------------------
# sources
#-----------------------------------
add_executable (${PROJECT_NAME}
  ${SRC_DIR}/bullet.c
  ${SRC_DIR}/bullet_manager.c
  ${SRC_DIR}/collision_manager.c
  ${SRC_DIR}/config.c
  ${SRC_DIR}/enemy.c
  ${SRC_DIR}/enemy_manager.c
  ${SRC_DIR}/game.c
  ${SRC_DIR}/input_component.c
  ${SRC_DIR}/in_game_state.c
  ${SRC_DIR}/particle.c
  ${SRC_DIR}/particle_manager.c
  ${SRC_DIR}/pause_state.c
  ${SRC_DIR}/player.c
  ${SRC_DIR}/text.c
  ${SRC_DIR}/tinymt32.c
  ${SRC_DIR}/util.c
  ${SRC_DIR}/wat.c
)


#-----------------------------------
# include headers
#-----------------------------------
target_include_directories (${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
target_include_directories (${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})


#-----------------------------------
# link libraries
#-----------------------------------
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
