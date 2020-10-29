#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <cmath>
#include <random>
#include <vector>
#include <iostream>

#include "Particle.hpp"
#include "Wall.hpp"

// TODO: Move these static functions to a utility namespace or something?

static bool circle_circle_intersection(const float ra, const float x0a, const float y0a, const float rb, const float x0b, const float y0b) {
    // Find the distance between the two centres
    const float d2 = (x0b - x0a) * (x0b - x0a) + (y0b - y0a) * (y0b - y0a);

    // We have an intersection if the distance is less than the sum of the radii
    // (here we use the metric squared because it is faster computationally)
    return d2 <= (ra + rb) * (ra + rb);
}

static bool circle_line_intersection(const float r, const Vec center, const Vec A, const Vec B) {
    Vec D = B - A;
    Vec F = A - center;

    float a = D.norm_sqr();
    float b = 2.f * F.dot(D);
    float c = F.norm_sqr() - r * r;

    float delta2 = b * b  - 4.f * a * c;

    if (delta2 < 0.f) return false;

    // TODO: We can get rid of this square root easily
    float delta = std::sqrt(delta2);
    float t1 = (-b - delta)/(2.f * a);
    float t2 = (-b + delta)/(2.f * a);

    return (t1 >= 0.f && t1 <= 1.f) || (t2 >= 0.f && t2 <= 1.f);
}

static Vec lennard_jones(const Vec A, const Vec B) {
    const float r2 = (B - A).norm_sqr();

    if (r2 < 0.1) return Vec(100000.f, 100000.f);

    if (r2 < 6.25f) { // equivalent to r < 2.5 (V(x > 2.5) ≈ 0)
        // TODO: Use the generalized form of the fast inverse square root
        // described here: http://h14s.p5r.org/2012/09/0x5f3759df.html
        // This can be used as style points because its a really neat algorithm
        const float r6_inv = 1.f/(r2 * r2 * r2);

        const float f_mag = -48.f * (r6_inv * r6_inv - 0.5f * r6_inv);
        return (B - A).normalized() * f_mag;
    }
    return Vec(0.f, 0.f);
}

// static const constexpr size_t num_particles = 100;
#define NUM_PARTICLES 1000

class Simulator {
    public:
        Simulator() {
            m_particles = std::vector<Particle>();
            m_walls = std::vector<Wall>();
        }

        Simulator(std::vector<Wall>& walls) {
            std::default_random_engine generator; // TODO: Seed the generator perhaps?
            std::normal_distribution<float> pos_dist(0.f, 0.1f);
            std::uniform_real_distribution<float> x_velo_dist(-45.f, 45.f);
            std::uniform_real_distribution<float> y_velo_dist(0.f, 15.f);

            for (size_t i = 0; i < NUM_PARTICLES; ++i) {
                const Vec pos = Vec(pos_dist(generator), pos_dist(generator));
                const Vec velo = Vec(x_velo_dist(generator), y_velo_dist(generator));
                Particle p(pos, velo);
                m_particles.push_back(p);
            }
            m_walls = walls;
        }

        void step(const float dt) {
            for (size_t i = 0; i < m_particles.size(); ++i) {
                const Particle p1 = m_particles[i];

                // Particles bounce off walls
                for (auto&& wall : m_walls) {
                    std::vector<Vec> verts = wall.get_vertices();
                    for (size_t j = 0; j < verts.size() - 1; ++j) {
                        Vec vert1 = verts[j];
                        Vec vert2 = verts[j+1];

                        if (circle_line_intersection(p1.get_radius(), p1.get_pos(), vert1, vert2)) {
                            Vec velo = p1.get_velo();
    
                            // F = dp / dt
                            wall.force += velo * (p1.get_mass() * 1.f/dt);

                            Vec normal = Vec(-(vert2 - vert1).y, (vert2-vert1).x).normalized();
                            velo -= normal * 2.f * velo.dot(normal);
                            m_particles[i].set_velo(velo);
                        }
                    }
                }

                // QUESTION: Particles actually shouldn't bounce off each other in an ideal gas, so we can assume ideal gas for performance
                // Particles bounce off of each other
                // for (size_t j = i+1; j < m_particles.size(); ++j) {
                    // const Particle p2 = m_particles[j];

                    // Forces from neighbouring particles
                    // Vec force = lennard_jones(p1.get_pos(), p2.get_pos());
                    // m_particles[i].impulse(force);
                    // m_particles[j].impulse(force * -1.f);

                    // TODO: Add forces from external fields
                // }
            }

            // Step the particles in time (we don't move the walls because we
            // assume they do not deform)
            for (size_t i = 0; i < m_particles.size(); ++i) {
                m_particles[i].step(dt);
            }
        }

        void many_step(const size_t num_steps, float dt) {
            for (size_t i = 0; i < num_steps; ++i) {
                step(dt);
            }
        }

        std::vector<Wall> get_walls() {
            return m_walls;
        }

        py::memoryview get_particle_positions() {
            float* positions = new float[m_particles.size() * 2];
            for (size_t i = 0; i < m_particles.size(); ++i) {
                positions[2*i] = m_particles[i].get_pos().x;
                positions[2*i + 1] = m_particles[i].get_pos().y;
            }
            m_allocations.push_back(positions);
            return py::memoryview::from_buffer(
                    positions,
                    {NUM_PARTICLES, 2}, // rows, cols TODO: replace NUM_PARTICLES with m_particles.size()
                    {2 * sizeof(float), sizeof(float)});
        }

        void free_allocation() {
            if (m_allocations.empty()) return;

            delete[] m_allocations[0];
            m_allocations.erase(m_allocations.begin(), m_allocations.begin() + 1);
        }

        ~Simulator() {
            for (auto alloc : m_allocations) {
                delete[] alloc;
            }
        }

    private:
        std::vector<Particle> m_particles;
        std::vector<Wall> m_walls;
        std::vector<float*> m_allocations;
};

void py_init_simulator(py::module& m) {
    py::class_<Simulator>(m, "Simulator")
        .def(py::init<>())
        .def(py::init<std::vector<Wall>&>())
        .def("step", &Simulator::step)
        .def("many_step", &Simulator::many_step)
        .def("get_particle_positions", &Simulator::get_particle_positions)
        .def("free_allocation", &Simulator::free_allocation)
        .def("get_walls", &Simulator::get_walls);
}

#endif /*__SIMULATOR_HPP__ */
