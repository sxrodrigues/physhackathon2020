#ifndef __WALL_HPP__
#define __WALL_HPP__

#include <random>
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

        void mutate(double max_dist) {
            std::default_random_engine generator(m_vertices[0].x);
            std::uniform_real_distribution<float> dist(-max_dist, max_dist);
            for (size_t i = 1; i < m_vertices.size(); ++i) {
                m_vertices[i].x += dist(generator);
                m_vertices[i].y += dist(generator);
            }
        }

        Vec force;

    private:
        std::vector<Vec> m_vertices;
};

void py_init_wall(py::module& m) {
    py::class_<Wall>(m, "Wall")
        .def(py::init<>())
        .def(py::init<std::vector<Vec>&>())
        .def("add_vertex", &Wall::add_vertex)
        .def_readwrite("force", &Wall::force)
        .def("get_vertices", &Wall::get_vertices)
        .def("mutate", &Wall::mutate);
}

#endif /* __WALL_HPP__ */

