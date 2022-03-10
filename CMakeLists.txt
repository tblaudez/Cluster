cmake_minimum_required(VERSION 3.16)
project(Cluster C)

set(CMAKE_VERBOSE_MAKEFILE true)
set(SOURCE src/cluster.c include/cluster.h src/utils.c src/rotation.c src/insertToken.c src/winner.c src/draw_mlx.c src/gameLoop.c)

add_executable(Cluster ${SOURCE})
target_include_directories(Cluster PRIVATE include MLX42/include)
target_link_libraries(Cluster -L${CMAKE_CURRENT_SOURCE_DIR}/MLX42/ -lmlx42 -ldl -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lbsd -lm)