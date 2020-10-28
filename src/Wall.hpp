#ifndef __WALL_HPP__
#define __WALL_HPP__

#include <vector>

#include "Vec.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Wall {
    public:
        Wall() {
            m_vertices = std::vector<Vec>();
        }
        Wall(const std::vector<Vec>& vertices) {
            m_vertices = vertices;
        }

        void add_vertex(const Vec& vertex) {
            m_vertices.push_back(vertex);
        }

        std::vector<Vec>& get_vertices() {
            return m_vertices;
        }

    private:
        std::vector<Vec> m_vertices;
};

void py_init_wall(py::module& m) {
    py::class_<Wall>(m, "Wall")
        .def(py::init<>())
        .def(py::init<std::vector<Vec>&>())
        .def("add_vertex", &Wall::add_vertex);
}

#endif /* __WALL_HPP__ */

