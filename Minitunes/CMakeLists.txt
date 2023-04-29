# CMakeList.txt: proyecto de CMake para Minitunes, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

project ("Minitunes")
set (CMAKE__CXX_STANDARD 20)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()


# Agregue un origen al ejecutable de este proyecto.
add_executable (Minitunes "Minitunes.cpp")

# TODO: Agregue pruebas y destinos de instalación si es necesario.
