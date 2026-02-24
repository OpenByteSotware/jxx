#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

template <typename T>
class Synchronized {
private:
    T data;
    mutable std::mutex mtx;

    // Proxy object that locks the mutex and gives access to T
    class LockedAccess {
    private:
        T* ptr;
        std::unique_lock<std::mutex> lock;
    public:
        LockedAccess(T* p, std::mutex& m) 
            : ptr(p), lock(m) {}
        
        T* operator->() { return ptr; }
        T& operator*()  { return *ptr; }
    };

    // Const proxy for read-only access
    class LockedAccessConst {
    private:
        const T* ptr;
        std::unique_lock<std::mutex> lock;
    public:
        LockedAccessConst(const T* p, std::mutex& m) 
            : ptr(p), lock(m) {}
        
        const T* operator->() const { return ptr; }
        const T& operator*()  const { return *ptr; }
    };

public:
    // Constructor forwarding args to T
    template <typename... Args>
    explicit Synchronized(Args&&... args) 
        : data(std::forward<Args>(args)...) {}

    // Non-const access
    LockedAccess operator->() {
        return LockedAccess(&data, mtx);
    }

    // Const access
    LockedAccessConst operator->() const {
        return LockedAccessConst(&data, mtx);
    }

    // Non-const dereference
    LockedAccess operator*() {
        return LockedAccess(&data, mtx);
    }

    // Const dereference
    LockedAccessConst operator*() const {
        return LockedAccessConst(&data, mtx);
    }
};
