cmake_minimum_required(VERSION 3.18.2)
project(simulator)

set(pybind11_DIR "/home/idwcorni/.local/lib/python3.8/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 2.6.0 REQUIRED)
pybind11_add_module(simulator SHARED src/module.cpp)
