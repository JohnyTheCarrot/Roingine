cmake_minimum_required(VERSION 3.26)

project(Duktape LANGUAGES C)

add_library(${PROJECT_NAME} duktape.c duktape.h duk_config.h)
