#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <random>
#include <vector>
#include <iostream>

#include "Particle.hpp"
#include "Wall.hpp"

static bool circle_circle_intersection(const float ra, const float x0a, const float y0a, const float rb, const float x0b, const float y0b) {
    // Find the distance between the two centers
    const float d2 = (x0b - x0a) * (x0b - x0a) + (y0b - y0a) * (y0b - y0a);

    // We have an intersection if the distance is less than the sum of the radii
    // (here we use the metric squared because it is faster computationally)
    return d2 <= (ra + rb) * (ra + rb);
}

static bool circle_line_intersection(const float r, const float x0, const float y0, const Vec2f A, const Vec2f B) {
    // Algorithm from:
    // https://mathworld.wolfram.com/Circle-LineIntersection.html

    // First, we adjust the circle and the points A and B such that the circle
    // is at the origin
    const Vec2f center = Vec2f(x0, y0);
    const Vec2f A_prime = A - center;
    const Vec2f B_prime = B - center;

    // Calculate D and d_r^2
    const float D = (A_prime.get_x() + B_prime.get_y() - B_prime.get_x() * A_prime.get_y());
    const float dr2 = (B_prime.get_x() - A_prime.get_x()) * (B_prime.get_x() - A_prime.get_x()) + (B_prime.get_y() - A_prime.get_y()) * (B_prime.get_y() - A_prime.get_y());

    return r*r * dr2 - D*D >= 0.f;
}

class Simulator {
    public:
        Simulator() {
            m_particles = std::vector<Particle2f>();
            m_walls = std::vector<Wall>();
        }

        Simulator(const size_t num_particles) {
            std::default_random_engine generator; // TODO: Seed the generator perhaps?
            std::uniform_real_distribution<float> distribution(-1.f, 1.f);

            for (size_t i = 0; i < num_particles; ++i) {
                Particle2f p(Vec2f(distribution(generator), distribution(generator)));
                p.set_velo(Vec2f(distribution(generator) * 7.f, distribution(generator)) * 7.f);
                m_particles.push_back(p);
            }
            m_walls = std::vector<Wall>();
        }

        void step(const float dt) {
            // Collisions
            for (size_t i = 0; i < m_particles.size(); ++i) {
                Particle2f p1 = m_particles[i];
                for (size_t j = i+1; j < m_particles.size(); ++j) {
                    Particle2f p2 = m_particles[j];

                    // Particles bounce off of each other
                    if (circle_circle_intersection(p1.get_radius(), p1.get_pos().get_x(), p1.get_pos().get_y(), p2.get_radius(), p2.get_pos().get_x(), p2.get_pos().get_y())) {
                        const Vec2f x1 = p1.get_pos();
                        Vec2f v1 = p1.get_velo();
                        const float m1 = p1.get_mass();
                        const Vec2f x2 = p2.get_pos();
                        Vec2f v2 = p2.get_velo();
                        const float m2 = p2.get_mass();

                        // Ref: https://www.wikiwand.com/en/Elastic_collision
                        v1 -= (x1 - x2) * (2*m2/(m1 + m2) * (v1 - v2).dot(x1 - x2) / (x1 - x2).norm_sqr());
                        v2 -= (x2 - x1) * (2*m1/(m2 + m1) * (v2 - v1).dot(x2 - x1) / (x2 - x1).norm_sqr());

                        m_particles[i].set_velo(v1);
                        m_particles[j].set_velo(v2);

                        std::cout << "Particle " << i << " and " << j << " bounced off each other\n";
                    }

                    // Particles bounce off walls
                    // TODO:
                }
            }

            // Add forces from external fields (we should create a Particle::impulse()
            // function to use for this)
            // TODO
            
            // Step the particles in time (we don't move the walls because we
            // assume they do not deform)
            for (size_t i = 0; i < m_particles.size(); ++i) {
                m_particles[i].step(dt);
            }
        }

        py::memoryview get_particle_positions() {
            float* positions = new float[m_particles.size() * 2];
            for (size_t i = 0; i < m_particles.size(); ++i) {
                positions[2*i] = m_particles[i].get_pos().get_x();
                positions[2*i + 1] = m_particles[i].get_pos().get_y();
            }
            m_allocations.push_back(positions);
            return py::memoryview::from_buffer(
                    positions,
                    {30, 2}, // rows, cols TODO: replace 10 with m_particles.size()
                    {2 * sizeof(float), sizeof(float)});
        }

        void free_allocation() {
            if (m_allocations.empty()) return;

            delete[] m_allocations[0];
            m_allocations.erase(m_allocations.begin(), m_allocations.begin() + 1);
        }

    private:
        std::vector<Particle2f> m_particles;
        std::vector<Wall> m_walls;
        std::vector<float*> m_allocations;
};

void py_init_simulator(py::module& m) {
    py::class_<Simulator>(m, "Simulator")
        .def(py::init<>())
        .def(py::init<const size_t>())
        .def("step", &Simulator::step)
        .def("get_particle_positions", &Simulator::get_particle_positions)
        .def("free_allocation", &Simulator::free_allocation);
}

#endif /*__SIMULATOR_HPP__ */
