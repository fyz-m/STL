#include <print>
#include <memory>


namespace lib {

template<typename T>
class unique_ptr {

private:

     T* m_ptr;

public:

    // Move constructor
    explicit unique_ptr(const T*&& ptr) : m_ptr{ptr} {}

    explicit unique_ptr(T* ptr) : m_ptr(ptr) {} 

    explicit unique_ptr() : m_ptr{nullptr} {}


    T* get() const {
        return m_ptr;
    }

    T* release() {
        auto p = m_ptr;
        m_ptr = nullptr;
        return p;
    }

    ~unique_ptr() {
        // Only call delete if ptr is not nullptr
        if (m_ptr)
            delete m_ptr;    
    }

    void reset(T* new_ptr) {
        this->~unique_ptr();  
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
        this->~unique_ptr();
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
    template<typename _T, typename ... Args>
    [[nodiscard]] unique_ptr<_T> 
    make_unique(Args&& ... args) {
        auto ptr = new _T(std::forward<Args>(args)...);
        if (!ptr) 
            throw std::bad_alloc(); 
        return unique_ptr<_T>(ptr);
    }

} // end namespace

class entity {
    public:
    std::string name;
    int id;

    explicit entity(std::string& name, int id) 
            : name{name}, id{id}
            {
                std::println("Copy constructor called");
            }

    explicit entity(std::string&& name, int id) 
            : name{std::move(name)}, id{id}
            {
                std::println("Move constructor called");
            } 
            
    ~entity() {std::println("Calling enttiy destructor.");}
};

int main() {
            
    auto p2 = lib::make_unique<entity>("Old name", 2);

    entity entity1("Name", 4);

    std::println("Name = {}", p2->name);        
    p2.reset(new entity("New name", 2));
    std::println("Name = {}", p2->name);        


}
