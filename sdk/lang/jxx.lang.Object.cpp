//#include "jxx.lang.ClassNotFoundException.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.ClassRegistration.h"
#include "jxx.lang.Class.h"
#include "jxx.lang.Cast.h"

using namespace jxx::lang;

Object::~Object() {
    // Break the cycle automatically
    releaseSelf();
}

jxx::Ptr<ClassAny> Object::getClass() const {
    const TypeInfo* ti = TypeRegistry::instance().findByType(std::type_index(typeid(*this)));
    return std::make_shared<ClassAny>(ti);
}

// Virtual clone method
jxx::Ptr<jxx::lang::Object> Object::clone() const {
    // Check if this object is Cloneable
    if (std::dynamic_pointer_cast<Cloneable>(thisPtr) == nullptr) {
        throw std::runtime_error("CloneNotSupportedException");
    }
    
    // If Cloneable, delegate to derived class's cloneImpl
    return cloneImpl();
}

void Object::releaseSelf() {
    // This method is called when the last shared_ptr to this object is destroyed.
    // this object is being destroyed since this method is called from the destructor
    thisPtr.reset();
}

// Java-like: logical equality (default identity)
jbool Object::equals(const jxx::Ptr<Object> other) const {
    return this == other.get();
}

// Java-like: hashCode (default identity-based)
jxx::lang::jint Object::hashCode() const {
return std::hash<const void*>{}(thisPtr.get());
}

// Class name (demangled where supported); override if you prefer custom names
jxx::Ptr<String> Object::getClassName() const {
    return JXX_NEW<String>(this->getClassName_());
}

// Java-like: "Class@hexHash"
jxx::Ptr<String>  Object::toString() const {
    std::ostringstream oss;
    oss << getClassName_() << "@0x" << std::hex << hashCode();
    return JXX_NEW<String>(oss.str());
}

// Identity check (reference equality)
bool Object::same(const Object& other) const {
    return this == &other;
}

void Object::wait() {
    std::unique_lock<std::mutex> lk(mtx_);
    cv_.wait(lk);
}

void Object::notify() {
    std::lock_guard<std::mutex> lg(mtx_);
    cv_.notify_one();
}

void Object::notifyAll() {
    std::lock_guard<std::mutex> lg(mtx_);
    cv_.notify_all();
}

jxx::Ptr<jxx::lang::Object> Object::cloneImpl() const {
    throw std::runtime_error("cloneImpl not implemented");
}
                
std::string Object::getClassName_() const {
#if defined(__GNUG__) || defined(__clang__) || defined(_MSC_VER)
    return std::string(demangle(typeid(*this).name()));
#else
    return std::string("Object");
#endif
}
