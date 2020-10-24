#ifndef __VEC_HPP__
#define __VEC_HPP__

#include <array>
#include <cstddef>
#include <type_traits>

template <typename T, size_t N>
class Vec {
    public:
        // 0-initialized constructor
        Vec() {
            m_data = {};
        }

        // Initialize from an array
        Vec(const std::array<T, N>&& data) {
            m_data = data;
        }

        // FIXME: We are assuming N >= 1
        // Base case for variadic constructor
        Vec(T x) {
            _construct(0, x);
        }

        // General case for variadic constructor
        template<typename... Args>
        Vec(T x, Args... args) {
            _construct(0, x, args...);
        }

        // This clever template guarding of using n == N used in get_*() comes
        // from:
        // https://stackoverflow.com/a/39154242/6026013

        // Return the x component if N >= 1
        template <size_t n = N>
        typename std::enable_if<n >= 1 && n == N, T>::type get_x() const {
            return m_data[0];
        }

        // Return the y component if N >= 2
        template <size_t n = N>
        typename std::enable_if<n >= 2 && n == N, T>::type get_y() const {
            return m_data[1];
        }

        // Return the z component if N >= 3
        template <size_t n = N>
        typename std::enable_if<n >= 3 && n == N, T>::type get_z() const {
            return m_data[2];
        }

        // Return the w component if N >= 4
        template <size_t n = N>
        typename std::enable_if<n >= 4 && n == N, T>::type get_w() const {
            return m_data[3];
        }

        Vec<T, N>& operator+=(const Vec<T, N>& rhs) {
            for (size_t i = 0; i < N; ++i) {
                m_data[i] += rhs.m_data[i];
            }
            return *this;
        }

        Vec<T, N> operator*(const T rhs) {
            std::array<T, N> new_data = m_data;
            for (auto&& elem : new_data) {
                new_data *= rhs;
            }
            return Vec<T, N>(new_data);
        }

        Vec<T, N>& operator*=(const T rhs) {
            for (auto&& elem : m_data) {
                elem *= rhs;
            }
            return *this;
        }

    private:
        void _construct(const size_t i, T x) {
            m_data[i] = x;
        }

        template<typename... Args>
        void _construct(const size_t i, T x, Args... args) {
            m_data[i] = x;
            _construct(i+1, args...);
        }

        std::array<T, N> m_data;
};

template<typename T, size_t N>
Vec<T, N> operator*(const T lhs, const Vec<T, N>& rhs) {
    std::array<T, N> new_data = rhs.m_data;
    for (auto&& elem : new_data) {
        new_data *= lhs;
    }
    return Vec<T, N>(new_data);
}

typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;

#endif /* __VEC_HPP__ */

