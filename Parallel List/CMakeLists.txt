﻿# CMakeList.txt: proyecto de CMake para Parallel List, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

project ("Parallel List")

# Agregue un origen al ejecutable de este proyecto.
add_executable (CMakeTarget "Parallel List.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Agregue pruebas y destinos de instalación si es necesario.
