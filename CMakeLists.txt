project(
  secret-string
  LANGUAGES CXX
  VERSION 0.1.0
  DESCRIPTION "A simple string literal obfuscation library")

add_library(secret-string INTERFACE)
target_include_directories(secret-string INTERFACE include)
target_compile_features(secret-string INTERFACE cxx_std_20)
set_target_properties(secret-string PROPERTIES CXX_EXTENSIONS OFF)
