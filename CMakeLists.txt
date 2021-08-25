cmake_minimum_required(VERSION 3.19)
project(asembler C)

set(CMAKE_C_STANDARD 99)

add_executable(asembler main.c config.c config.h errors.h memory_manager.c memory_manager.h error_handler.c error_handler.h label.c label.h output_manager.c output_manager.h encoder.c encoder.h service.c service.h input_validator.c input_validator.h)