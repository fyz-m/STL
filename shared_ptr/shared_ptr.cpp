#include <cstddef>
#include <print>
#include <string>
#include <utility>
#include <memory>

namespace lib {

template<typename T>
class shared_ptr {

    private:
        T* m_Ptr{};
        size_t* m_Ptr_count{};

    public:

        shared_ptr() : m_Ptr{nullptr}, m_Ptr_count{nullptr} {}

        explicit shared_ptr(T* other) : m_Ptr{other} 
                {
                    m_Ptr_count = new size_t(1);
                }

        // Copy constructor
        shared_ptr(const shared_ptr& other) 
                  : m_Ptr{other.get()}, m_Ptr_count(other.m_Ptr_count) 
                  {
                    incrementCount();
                  }
        
        // Move constructor
        shared_ptr(shared_ptr&& other)
                  : m_Ptr(other.get()), m_Ptr_count(other.m_Ptr_count) 
                  {
                    // Set to nullptr so other's destructor doesn't decrement count
                    other.m_Ptr_count = nullptr;
                    other.m_Ptr = nullptr;
                  }

        
        ~shared_ptr() {
            if (m_Ptr_count && --(*m_Ptr_count) == 0) {
                delete m_Ptr;
                delete m_Ptr_count;
            }
        }

        // Copy assignment
        shared_ptr& operator=(const shared_ptr& other) {
            if (this != &other) {
                reset();            // Release ownership of current ptr
                copyFrom(other);    // Copy members of other
                incrementCount();   
            }
            return *this;           
        }

        // Move assignment
        shared_ptr& operator=(shared_ptr&& other) {
            if (this != &other) {
            reset();
            copyFrom(other);
            other.m_Ptr = nullptr;
            other.m_Ptr_count = nullptr;
            }
            return *this;
        }

        T* get() const {
            return m_Ptr;
        }

        // Release ownership and set members to null
        void reset() {

            // If already nullptr
            if (!m_Ptr_count)
                return; 

            // If *this is the only ptr to obj
            else if (isUnique()) {
                delete m_Ptr;
                delete m_Ptr_count;
            }
            else 
                --*m_Ptr_count;

            // Set to nullptr so destructor is avoided
            m_Ptr_count = nullptr;
            m_Ptr = nullptr;
        }

        // Release and own another ptr
        void reset(T* other) {

            if (m_Ptr_count) {                 
                if (isUnique()) {
                    delete m_Ptr;
                    delete m_Ptr_count;
                }
                else 
                    --*m_Ptr_count;
            }
            m_Ptr = other;
            m_Ptr_count = new size_t{1};
        }

        // Swap resources wih another shared_ptr
        void swap(shared_ptr& other) {
            auto this_ptr = m_Ptr;
            auto this_Ptr_count = m_Ptr_count;
            copyFrom(other);

            other.m_Ptr = this_ptr;
            other.m_Ptr_count = this_Ptr_count;
        }

        size_t useCount() const {
            if (m_Ptr_count)
                return *m_Ptr_count;
            return 0;
        }

        T& operator*() const {
            return *m_Ptr;
        }

        T* operator->() const {
            return m_Ptr;
        }

        explicit operator bool() const {
            return m_Ptr;
        }

private:
        // Copy members of other shared_ptr
        void copyFrom(const shared_ptr& other) {
            this->m_Ptr = other.m_Ptr;
            this->m_Ptr_count = other.m_Ptr_count;
        }


        void incrementCount() {
            if (m_Ptr_count) {
                ++*m_Ptr_count;
                return;
            }   
            m_Ptr_count = new size_t(1);
        }

        void decrementCount() {
            if (m_Ptr_count)
                --*m_Ptr_count;
            }

        bool isUnique() const {
            if (useCount() == 1) 
                return true;
            return false;
        }
 };

template<typename T, typename ...Args>
[[nodiscard]] shared_ptr<T> make_shared(Args&& ...args) {
    return shared_ptr(new T(std::forward<Args>(args)...));
 }

} // end namespace

class entity {
public:
    std::string name;

    entity(std::string name) : name{name} {}

    ~entity() {
        std::println("Entity Destructor called.");
    }
};
int main () {
{
    auto p1 = lib::make_shared<entity>("Name");
    std::println("Ref count = {}", p1.useCount());
    lib::shared_ptr<entity> p2 = lib::make_shared<entity>("n1");
    p2 = p1;
    std::println("Ref count = {}", p1.useCount());
    auto p3(p2);
    std::println("Ref count = {}", p1.useCount());

}
    
    std::println("Outside block");

}