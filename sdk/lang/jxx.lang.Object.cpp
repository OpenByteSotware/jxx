//#include "jxx.lang.ClassNotFoundException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.ClassRegistration.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Cast.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"


namespace jxx::lang {

    Object::~Object() {
        // Break the cycle automatically
        releaseSelf();
    }

    // Copy constructor: default-construct mutex and cv since they cannot be copied
    Object::Object(const Object& /*other*/)
        : std::enable_shared_from_this<Object>()
        , thisPtr_()
        , mtx_()
        , cv_()
        , mutex_()
    {
    }

    // Copy assignment: similar to copy constructor
    Object& Object::operator=(const Object& other) {
        if (this != &other) {
            // Don't copy thisPtr_, mtx_, cv_, or mutex_ - they represent this object's identity
            // Derived classes will handle their own member copying through their copy assignment
        }
        return *this;
    }

    jxx::Ptr<jxx::lang::ClassAny> Object::getClass() const {
        // Exact JXX semantics: runtime class of the *dynamic* object.
        // Requires RTTI enabled (typeid on polymorphic type).
        return ClassAny::forType(std::type_index(typeid(*this)));
    }

    // Virtual clone method
    jxx::Ptr<jxx::lang::Object> Object::clone() const {
        // Check if this object is Cloneable
        if (std::dynamic_pointer_cast<Cloneable>(thisPtr()) == nullptr) {
            throw std::runtime_error("CloneNotSupportedException");
        }

        // If Cloneable, delegate to derived class's cloneImpl
        return cloneImpl();
    }

    jxx::Ptr<Object> Object::thisPtr() const {
        if (auto existing = thisPtr_.lock()) {
            return existing;
        }

        try {
            return std::const_pointer_cast<Object>(shared_from_this());
        }
        catch (const std::bad_weak_ptr&) {
            throw std::logic_error(
                "Object is not owned by jxx::NEW");
        }
    }

    void Object::releaseSelf() {
        // This method is called when the last shared_ptr to this object is destroyed.
        // this object is being destroyed since this method is called from the destructor
        thisPtr_.reset();
    }

    // JXX-style: logical equality (default identity)
    jbool Object::equals(const jxx::Ptr<Object>& other) const {
        return this == other.get();
    }

    // JXX-style: hashCode (default identity-based)
    jxx::lang::jint Object::hashCode() const {
        return std::hash<const void*>{}(this);
    }

    // Class name (demangled where supported); override if you prefer custom names
    jxx::Ptr<String> Object::getClassName() const {
        return this->getClassName_();
    }

    // JXX-style: "Class@hexHash"
    jxx::Ptr<jxx::lang::String>  Object::toString() const {
        std::ostringstream oss;
        oss << getClassName_() << "@0x" << std::hex << hashCode();
        return jxx::NEW<jxx::lang::String>(oss.str());
    }

    // Identity check (reference equality)
    bool Object::same(const jxx::Ptr<Object>& other) const {
        return this == other.get();
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

    jxx::Ptr<jxx::lang::String> Object::getClassName_() const {
#if defined(__GNUG__) || defined(__clang__) || defined(_MSC_VER)
        return jxx::NEW<String>(demangle(typeid(*this).name()));
#else
        return jxx::NEW<String>("Object");
#endif
    }

}