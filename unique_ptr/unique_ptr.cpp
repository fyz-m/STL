#include <cstddef>
#include <utility>

namespace lib {

template <typename T>
class unique_ptr {

  private:
    T* m_ptr;

  public:
    // Delete the copy constructor
    unique_ptr(const unique_ptr& ptr) = delete;

    // Delete copy assignment
    void operator=(const unique_ptr& ptr) = delete;

    // Move constructor
    // Transfer ownership of resources
    unique_ptr(unique_ptr&& other)
        : m_ptr{other.get()} {
        other.m_ptr = nullptr;
    }

    // Constructor from raw pointer
    explicit unique_ptr(T* other)
        : m_ptr{other} {}

    explicit unique_ptr()
        : m_ptr{nullptr} {}

    // Return managed pointer
    T* get() const { return m_ptr; }

    // Release ownership of pointer (no longer automatically managed)
    T* release() {
        auto old_p = m_ptr;
        m_ptr = nullptr;
        return old_p;
    }

    // Free managed object
    ~unique_ptr() { delete m_ptr; }

    // Point to another object, freeing the old one
    // If no pointer is provided
    void reset(T* new_ptr = nullptr) {
        if (m_ptr)
            delete m_ptr;
        m_ptr = new_ptr;
    }

    T& operator*() const { return *m_ptr; }

    T* operator->() const { return m_ptr; }

    // Move assignment
    unique_ptr& operator=(unique_ptr<T>&& other) {
        // Prevent self-move
        if (this != &other) {
            this->reset(other.get());
            other.m_ptr = nullptr;
        }
        return *this;
    }

    std::nullptr_t operator=(std::nullptr_t) {
        delete m_ptr;
        m_ptr = nullptr;
        return nullptr;
    }

    // Comparison operators overload
    bool operator==(const unique_ptr<T>& p) const { return this->get() == p.get(); }

    bool operator!=(const unique_ptr<T>& p) const { return this->get() != p.get(); }

    bool operator>=(const unique_ptr<T>& p) const { return this->get() >= p.get(); }

    bool operator<=(const unique_ptr<T>& p) const { return this->get() <= p.get(); }

    bool operator>(const unique_ptr<T>& p) const { return this->get() > p.get(); }

    bool operator<(const unique_ptr<T>& p) const { return this->get() < p.get(); }
};

template <typename T, typename... Args>
[[nodiscard]] unique_ptr<T> make_unique(Args&&... args) {
    auto ptr = new T(std::forward<Args>(args)...);
    return unique_ptr<T>(ptr);
}

} // namespace lib
