#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include "Vec.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

template<typename T, size_t N>
class Particle {
    public:
        Particle() {
            m_pos = Vec<T, N>();
            m_velo = Vec<T, N>();
            m_accel = Vec<T, N>();
            m_mass = 1.f;
            m_restitution = 1.f;
        }

        Particle(const Vec<T, N>& pos) : m_pos(pos) {
            m_velo = Vec<T, N>();
            m_accel = Vec<T, N>();
            m_mass = 1.f;
            m_restitution = 1.f;
        }

        // Step forward in time
        void step(float dt) {
            // TODO: We need to use something better than the Euler method in
            // the future. Perhaps Verlet?

            // Simple Euler integration
            m_velo += m_accel * dt;
            m_pos += m_velo * 0.5f * dt;

            // Reset the acceleration to 0
            m_accel = Vec<T, N>();
        }

    private:
        Vec<T, N> m_pos;
        Vec<T, N> m_velo;
        Vec<T, N> m_accel;
        T m_mass;
        T m_restitution;
};

typedef Particle<float, 2> Particle2f;
typedef Particle<float, 3> Particle3f;

void py_init_particle(py::module& m) {
    py::class_<Particle2f>(m, "Particle2f")
        .def(py::init<>())
        .def(py::init<const Vec2f&>())
        .def("step", &Particle2f::step);

    py::class_<Particle3f>(m, "Particle3f")
        .def(py::init<>())
        .def(py::init<const Vec3f&>())
        .def("step", &Particle3f::step);
}

#endif /* __PARTICLE_HP__ */

