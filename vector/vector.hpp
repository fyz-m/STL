#include <cstddef>
#include <utility>

namespace lib {

template <typename T>
class Vector {

  private:
    // Number of elements in use
    size_t m_Size{0};

    // Total number of elements that memory is allocated for
    size_t m_Capacity{m_Size};

    // Pointer to buffer being used
    T* m_Buffer = static_cast<T*>((::operator new(m_Capacity * sizeof(T))));

  public:
    Vector() = default;

    Vector(size_t N)
        : m_Size(N) {}

    Vector(size_t N, const T& initVal)
        : m_Size(N) {
        fill(initVal);
    }

    Vector(size_t N, T&& initVal)
        : m_Size(N) {
        fill(std::move(initVal));
    }

    // Copy constructor
    Vector(const Vector<T>& other)
        : m_Size{other.size()} {
        copyElementsFrom(other);
    }

    // Copy assignment
    Vector<T>& operator=(const Vector<T>& other) {
        if (this == &other)
            return *this;

        freeResources();
        m_Size = other.size();
        m_Capacity = other.capacity();
        m_Buffer = static_cast<T*>(::operator new(m_Capacity * sizeof(T)));
        copyElementsFrom(other);
        return *this;
    }

    // Move constructor
    Vector(Vector<T>&& other) noexcept
        : m_Size{other.size()}, m_Capacity(other.capacity()), m_Buffer{other.data()} {
        other.m_Buffer = nullptr;
    }

    // Move assignment
    Vector<T>& operator=(Vector<T>&& other) noexcept {
        if (this == &other)
            return *this;

        freeResources();
        m_Size = other.size();
        m_Capacity = other.capacity();
        m_Buffer = other.data();
        other.m_Buffer = nullptr;
        return *this;
    }

    ~Vector() {
        freeResources();
    }

    void reserve(const size_t new_capacity) {

        if (new_capacity <= capacity())
            return;

        T* old_buffer = m_Buffer;
        m_Buffer = static_cast<T*>((::operator new(new_capacity * sizeof(T))));
        m_Capacity = new_capacity;

        for (size_t i = 0; i < size(); ++i) {
            // Call T's default constructor to initialize the sizeof(T) bytes at address
            // m_Buffer[i] We need to call the default constructor because T's copy/move
            // assignment assumes that m_Buffer[i] is an existing T object and will access that
            // object's members
            //
            // For example, if T is shared_ptr, operator=() will attempt to dereference the
            // bytes in m_Buffer[i] and possibly delete it. If those bytes are garbage data we
            // get UB, so we must initialize those bytes to their default by calling T's default
            // constructor
            new (&m_Buffer[i]) T();
            m_Buffer[i] = std::move(old_buffer[i]);
            old_buffer[i].~T();
        }

        ::operator delete(old_buffer);
    }

    void push_back(const T& val) {
        if (m_Size >= m_Capacity) {
            reserve(defaultResize());
        }
        ::new (&m_Buffer[m_Size]) T();
        m_Buffer[m_Size] = val;
        m_Size++;
    }

    void push_back(T&& val) {
        if (m_Size >= m_Capacity) {
            reserve(defaultResize());
        }
        ::new (&m_Buffer[m_Size]) T();
        m_Buffer[m_Size] = std::move(val);
        m_Size++;
    }

    void pop_back() {
        if (size() > 0) {
            back().~T();
            --m_Size;
        }
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (m_Size >= m_Capacity) {
            reserve(defaultResize());
        }
        ::new (&m_Buffer[m_Size]) T(std::forward<Args>(args)...);
        m_Size++;
    }

    void fill(const T& val) {
        for (size_t i = 0; i < size(); i++) {
            m_Buffer[i] = val;
        }
    }

    void fill(const T&& val) {
        for (size_t i = 0; i < size(); i++) {
            m_Buffer[i] = std::move(val);
        }
    }

    void clear() {
        for (size_t i = 0; i < size(); i++) {
            m_Buffer[i].~T();
        }
        m_Size = 0;
    }

    [[nodiscard]] size_t size() const {
        return m_Size;
    };
    [[nodiscard]] size_t capacity() const {
        return m_Capacity;
    };

    [[nodiscard]] bool empty() const {
        return size() == 0;
    }

    [[nodiscard]] T* data() const {
        return m_Buffer;
    }

    [[nodiscard]] T& front() const {
        return m_Buffer[0];
    }

    [[nodiscard]] T& back() const {
        return m_Buffer[m_Size - 1];
    }

    T& operator[](size_t index) const {
        return m_Buffer[index];
    }

  private:
    [[nodiscard]] size_t defaultResize() const {
        if (!size())
            return 10;
        return size() * 2;
    }

    void freeResources() {

        for (size_t i = 0; i < size(); ++i) {
            m_Buffer[i].~T();
        }
        if (m_Buffer)
            ::operator delete(m_Buffer);
    }

    void copyElementsFrom(const Vector<T>& other) {
        for (size_t i = 0; i < size(); ++i) {
            ::new (&m_Buffer[i]) T();
            m_Buffer[i] = other[i];
        }
    }
};

} // namespace lib