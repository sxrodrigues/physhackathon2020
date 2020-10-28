#ifndef __VEC_HPP__
#define __VEC_HPP__

#include <array>
#include <cmath>
#include <cstddef>
#include <type_traits>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

struct Vec {
        // 0-initialized constructor
        Vec() {
            x = 0.f;
            y = 0.f;
        }

        Vec(float x, float y) :
            x(x),
            y(y) {}

        Vec operator-(const Vec&rhs) const {
            return Vec(x - rhs.x, y - rhs.y);
        }

        Vec& operator-=(const Vec& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Vec operator+(const Vec& rhs) const {
            return Vec(x + rhs.x, y + rhs.y);
        }

        Vec& operator+=(const Vec& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vec operator*(const float rhs) const {
            return Vec(x * rhs, y * rhs);
        }

        Vec& operator*=(const float rhs) {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        Vec normalized() const {
            return *this * (1.f/std::sqrt(norm_sqr()));
        }

        float norm() const {
            return std::sqrt(x*x + y*y);
        }

        float norm_sqr() const {
            return x*x + y*y;
        }

        float dot(const Vec& rhs) {
            return x*rhs.x + y*rhs.y;
        }

        float x;
        float y;
};

void py_init_vec(py::module& m) {
    py::class_<Vec>(m, "Vec")
        .def(py::init<>())
        .def(py::init<const float, const float>());
}

#endif /* __VEC_HPP__ */

