#include <cstddef>
#include <stdexcept>

namespace lib {
template <typename T, size_t N>
class Array {

  private:
    T m_Buffer[N];

  public:
    T& operator[](const size_t index) {
        return m_Buffer[index];
    }

    const T& operator[](const size_t index) const {
        return m_Buffer[index];
    }

    T* data() {
        return m_Buffer;
    }

    const T* data() const {
        return m_Buffer;
    }

    T& front() {
        return m_Buffer[0];
    }

    const T& front() const {
        return m_Buffer[0];
    }

    T& back() {
        return m_Buffer[N - 1];
    }

    const T& back() const {
        return m_Buffer[N - 1];
    }

    T& at(const int index) {
        if (index < 0 || index >= N)
            throw std::out_of_range("Out of bounds array access.");
        return m_Buffer[index];
    }

    const T& at(const int index) const {
        if (index < 0 || index >= N)
            throw std::out_of_range("Out of bounds array access.");
        return m_Buffer[index];
    }

    void fill(const T& value) {
        for (int i = 0; i < N; ++i) {
            m_Buffer[i] = value;
        }
    }

    void swap(Array<T, N>& other) noexcept {
        for (int i = 0; i < N; ++i) {
            auto temp = std::move(m_Buffer[i]);
            m_Buffer[i] = std::move(other[i]);
            other[i] = std::move(temp);
        }
    }

    [[nodiscard]] constexpr size_t size() const {
        return N;
    }

    [[nodiscard]] constexpr size_t max_size() const {
        return N;
    }

    [[nodiscard]] constexpr bool empty() const {
        return N == 0;
    }
};
} // namespace lib