
#include <cstddef>
#include <stdexcept>
namespace lib {

template <typename T, size_t N>
class Array {

  private:
    T* m_Buffer{new T[N]};

  public:
    Array() = default;

    // Copy constructor
    Array(const Array<T, N>& other)
        : m_Buffer(new T[N]) {
        copyElementsFrom(other);
    }

    // Copy assignment
    Array& operator=(const Array<T, N>& other) {
        if (this == &other) {
            return *this;
        }

        // delete old array
        delete[] m_Buffer;
        m_Buffer = new T[N];

        copyElementsFrom(other);
        return *this;
    }

    // Move constructor
    Array(Array<T, N>&& other) noexcept
        : m_Buffer{other.m_Buffer} {
        other.m_Buffer = nullptr;
    }

    // Move assignment
    Array& operator=(Array<T, N>&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] m_Buffer;
        m_Buffer = other.m_Buffer;
        other.m_Buffer = nullptr;
        return *this;
    }

    T& operator[](size_t index) const {
        return m_Buffer[index];
    }

    ~Array() {
        delete[] m_Buffer;
    }

    T* data() const {
        return m_Buffer;
    }

    T& front() const {
        return m_Buffer[0];
    }

    T& back() const {
        return m_Buffer[N - 1];
    }

    T& at(int index) const {
        if (index < 0 || index >= N)
            throw std::out_of_range("Out of bounds array access.");
        return m_Buffer[index];
    }

    void fill(const T& value) {
        for (int i = 0; i < N; ++i) {
            m_Buffer[i] = value;
        }
    }

    [[nodiscard]] size_t size() const {
        return N;
    }

    [[nodiscard]] size_t max_size() const {
        return N;
    }

    [[nodiscard]] bool empty() const {
        return N == 0;
    }

  private:
    void copyElementsFrom(const Array<T, N>& other) const {
        for (int i = 0; i < N; i++) {
            m_Buffer[i] = other.m_Buffer[i];
        }
    }
};
} // namespace lib