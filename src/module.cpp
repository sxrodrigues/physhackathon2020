#include "Particle.hpp"
#include "Simulator.hpp"
#include "Vec.hpp"
#include "Wall.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(simulator, m) {
    m.doc() = "A simulator module";

    py_init_vec(m);
    py_init_particle(m);
    py_init_wall(m);
    py_init_simulator(m);
}
