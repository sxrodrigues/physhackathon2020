#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include <cmath>

#include "Vec.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class Particle {
    public:
        Particle() {
            m_pos = Vec(0.f, 0.f);
            m_prev_accel = Vec(0.f, 0.f);
            m_accel = Vec(0.f, 0.f);
            m_radius = 0.05f;
            m_mass = 1.f;
        }

        Particle(const Vec& pos, const Vec& velo) : 
            m_pos(pos)
        {
            m_velo = velo;
            m_prev_accel = Vec(0.f, 0.f);
            m_accel = Vec(0.f, 0.f);
            m_radius = 0.05f;
            m_mass = 1.f;
        }

        // Step forward in time (velocity Verlet)
        void step(const float dt) {
            m_pos += m_velo * dt + m_prev_accel * (0.5 * dt * dt);
            m_velo += (m_prev_accel + m_accel) * (0.5 * dt);

            m_accel = Vec(0.f, 0.f);
        }

        // Apply a force
        void impulse(const Vec& force) {
            m_prev_accel = m_accel;
            m_accel += force * (1.f / m_mass);
        }

        Vec get_pos() const {
            return m_pos;
        }

        Vec get_velo() const {
            return m_velo;
        }

        void set_velo(const Vec& velo) {
            m_velo = velo;
        }

        float get_radius() const {
            return m_radius;
        }

        float get_mass() const {
            return m_mass;
        }

    private:
        Vec m_pos;
        Vec m_velo;
        Vec m_accel;
        Vec m_prev_accel;
        float m_radius;
        float m_mass;
};

void py_init_particle(py::module& m) {
    py::class_<Particle>(m, "Particle")
        .def(py::init<>())
        .def(py::init<const Vec&, const Vec&>())
        .def("step", &Particle::step);
}

#endif /* __PARTICLE_HP__ */

