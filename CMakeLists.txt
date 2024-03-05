cmake_minimum_required(VERSION 3.25.0)

project(ctfp)

add_library(
  ${PROJECT_NAME}
  INTERFACE
)

add_library(
  ${PROJECT_NAME}::${PROJECT_NAME}
  ALIAS
  ${PROJECT_NAME}
)

target_compile_features(
  ${PROJECT_NAME}
  INTERFACE
  cxx_std_23
)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
  include
)
