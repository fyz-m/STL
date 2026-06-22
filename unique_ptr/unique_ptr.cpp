#include <print>
#include <memory>


namespace lib {

template<typename T>
class unique_ptr {

private:

     T* m_ptr;

public:

    // Move constructor
    // Transfer ownership of resources
    explicit unique_ptr(unique_ptr&& other) 
            : m_ptr{other.get()}
        {
            other.m_ptr = nullptr;
        }
    
    // Constructors from raw pointer
    explicit unique_ptr(T&& other) : m_ptr{other} {}

    explicit unique_ptr(T* ptr) : m_ptr(ptr) {} 
    
    // Default constructor
    explicit unique_ptr() : m_ptr{nullptr} {}

    // Return managed pointer
    T* get() const {
        return m_ptr;
    }

    // Release ownership of pointer (no longer automatically managed) 
    T* release() {
        auto p = m_ptr;
        m_ptr = nullptr;
        return p;
    }

    // Free managed object
    ~unique_ptr() {    
        delete m_ptr;    
    }   

    // Point to another object, freeing the old one
    void reset(T* new_ptr) {
        delete m_ptr;  
        m_ptr = new_ptr;
    }

    T& operator*() const {
        return *m_ptr;
    }

    T* operator->() const {
        return m_ptr;
    }

    // Move assignment
    void operator=(unique_ptr<T>&& p) {
        delete m_ptr;
        m_ptr = p.get();
        p = nullptr;
    }    

    void operator=(std::nullptr_t) {
        m_ptr = nullptr;
    }

    // Comparison operators overload
    bool operator==(const unique_ptr<T> p) const {
        return this->get() == p.get();
    }

    bool operator!=(const unique_ptr<T> p) const {
        return this->get() != p.get();
    }

    bool operator>=(const unique_ptr<T> p) const {
        return this->get() >= p.get();
    }

    bool operator<=(const unique_ptr<T> p) const {
        return this->get() <= p.get();
    }

    bool operator>(const unique_ptr<T> p) const {
        return this->get() > p.get();
    }

    bool operator<(const unique_ptr<T> p) const {
        return this->get() < p.get();
    }

    // Delete the copy constructor
    unique_ptr(const unique_ptr& ptr) = delete;

    // Delete copy assignment 
    void operator=(const unique_ptr& ptr) = delete;

};
    template<typename T, typename ... Args>
    [[nodiscard]] unique_ptr<T> 
    make_unique(Args&& ... args) {
        auto ptr = new T(std::forward<Args>(args)...); 
        return unique_ptr<T>(ptr);
    }

} // end namespace

class entity {
    public:
    std::string name;
    int id;

    explicit entity(std::string& name, int id) 
            : name{name}, id{id}
            {
                // std::println("Copy constructor called");
            }

    explicit entity(std::string&& name, int id) 
            : name{std::move(name)}, id{id}
            {
                // std::println("Move constructor called");
            } 
            
    // ~entity() {std::println("Calling enttiy destructor.");}
};

int main() {
    


    auto p2 {new int(2)}; 
    std::println("{}", *p2);
}
