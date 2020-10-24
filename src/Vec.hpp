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
        typename std::enable_if<n >= 1 && n == N, T>::type get_x() {
            return m_data[0];
        }

        // Return the y component if N >= 2
        template <size_t n = N>
        typename std::enable_if<n >= 2 && n == N, T>::type get_y() {
            return m_data[1];
        }

        // Return the z component if N >= 3
        template <size_t n = N>
        typename std::enable_if<n >= 3 && n == N, T>::type get_z() {
            return m_data[2];
        }

        // Return the w component if N >= 4
        template <size_t n = N>
        typename std::enable_if<n >= 4 && n == N, T>::type get_w() {
            return m_data[3];
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

typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;

