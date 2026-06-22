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
     unique_ptr(unique_ptr&& other) 
            : m_ptr{other.get()}
        {
            other.m_ptr = nullptr;
        }

    // Constructor from raw pointer
    explicit unique_ptr(T* other) : m_ptr(other) {} 
    
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
    // If no pointer is provided 
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
    unique_ptr& operator=(unique_ptr<T>&& other) {
        // Prevent self-move
        if (other != *this) {
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
    bool operator==(const unique_ptr<T>& p) const {
        return this->get() == p.get();
    }

    bool operator!=(const unique_ptr<T>& p) const {
        return this->get() != p.get();
    }

    bool operator>=(const unique_ptr<T>& p) const {
        return this->get() >= p.get();
    }

    bool operator<=(const unique_ptr<T>& p) const {
        return this->get() <= p.get();
    }

    bool operator>(const unique_ptr<T>& p) const {
        return this->get() > p.get();
    }

    bool operator<(const unique_ptr<T>& p) const {
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
    auto e1 = lib::make_unique<entity>("E1", 1);
    auto e2 = lib::make_unique<entity>("E2", 1);
    std::println("{}", e1->name);
    e1 = std::move(e2);
    std::println("After move : {}", e1->name);
    if (!e2.get()) 
        std::println("E2 is now nullptr");

    e1 = nullptr;
    if (!e1.get()) 
        std::println("E2 is now nullptr");

}
