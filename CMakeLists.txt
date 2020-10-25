cmake_minimum_required(VERSION 3.18.2)
project(simulator)

find_package(pybind11 REQUIRED)
pybind11_add_module(simulator src/module.cpp)
