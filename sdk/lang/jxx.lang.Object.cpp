//#include "jxx.lang.ClassNotFoundException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.ClassRegistration.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Cast.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalMonitorStateException.h"
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
        , monitorMutex_()
        , monitorCondition_()
        , monitorOwner_()
        , monitorDepth_(0U)
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
        // Exact runtime class of the dynamic object.
        // ClassInfo-backed classes register themselves through their Class()
        // path; Object itself is registered by the bootstrap descriptor.
        try {
            return ClassAny::forType(std::type_index(typeid(*this)));
        }
        catch (const IllegalStateException&) {
            if (typeid(*this) == typeid(Object)) {
                return class_info_detail::ensureObjectRegistered();
            }
            throw;
        }
    }

    ::jxx::lang::jbool Object::instanceOf(
        const jxx::Ptr<jxx::lang::ClassAny>& target) const noexcept
    {
        if (target == nullptr) return false;

        try {
            const auto source = getClass();
            return source != nullptr && target->isAssignableFrom(source);
        }
        catch (...) {
            // Type tests must not propagate registration/lookup failures.
            return false;
        }
    }

    // Virtual clone method
    jxx::Ptr<jxx::lang::Object> Object::clone() const {
        // Check if this object is Cloneable
        if (std::dynamic_pointer_cast<Cloneable>(thisPtr()) == nullptr) {
            throw CloneNotSupportedException();
        }

        // If Cloneable, delegate to derived class's cloneImpl
        return cloneImpl();
    }

    jxx::Ptr<Object> Object::thisPtr() const
    {
        auto object = thisPtr_.lock();

        if (object == nullptr) {
            throw std::logic_error(
                "Object self reference is not initialized");
        }

        return object;
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

    jxx::Ptr<jxx::lang::String> Object::toString() const {
        const auto runtimeClass = getClass();
        const auto className = runtimeClass == nullptr
            ? getClassName_()
            : runtimeClass->getName();

        std::ostringstream output;
        output << className->utf8()
               << '@'
               << std::hex
               << static_cast<std::uint32_t>(hashCode());
        return jxx::NEW<jxx::lang::String>(output.str());
    }

    // Identity check (reference equality)
    bool Object::same(const jxx::Ptr<Object>& other) const {
        return this == other.get();
    }

    void Object::verifyMonitorOwner_() const {
        if (monitorDepth_ == 0U ||
            monitorOwner_ != std::this_thread::get_id()) {
            throw IllegalMonitorStateException();
        }
    }

    bool Object::waitFor_(const std::chrono::nanoseconds& duration) {
        verifyMonitorOwner_();

        const std::size_t savedDepth = monitorDepth_;
        monitorDepth_ = 0U;
        monitorOwner_ = std::thread::id{};

        // synchronized() owns the recursive mutex once per entry. Retain one
        // adopted level for condition_variable_any and temporarily release any
        // additional reentrant levels while waiting.
        for (std::size_t depth = 1U; depth < savedDepth; ++depth) {
            monitorMutex_.unlock();
        }

        std::unique_lock<std::recursive_mutex> lock(
            monitorMutex_,
            std::adopt_lock);

        bool notified = true;
        if (duration == std::chrono::nanoseconds::zero()) {
            monitorCondition_.wait(lock);
        }
        else {
            notified = monitorCondition_.wait_for(lock, duration) !=
                std::cv_status::timeout;
        }

        for (std::size_t depth = 1U; depth < savedDepth; ++depth) {
            monitorMutex_.lock();
        }

        monitorOwner_ = std::this_thread::get_id();
        monitorDepth_ = savedDepth;
        (void)lock.release();
        return notified;
    }

    void Object::wait() {
        (void)waitFor_(std::chrono::nanoseconds::zero());
    }

    void Object::wait(::jxx::lang::jlong timeoutMillis) {
        wait(timeoutMillis, 0);
    }

    void Object::wait(
        ::jxx::lang::jlong timeoutMillis,
        ::jxx::lang::jint nanos) {

        if (timeoutMillis < 0 || nanos < 0 || nanos > 999999) {
            throw IllegalArgumentException();
        }

        if (timeoutMillis == 0 && nanos == 0) {
            wait();
            return;
        }

        // The Java 8 specification rounds any positive nanosecond remainder
        // up to one additional millisecond.
        const auto roundedMillis =
            nanos > 0 && timeoutMillis <
                std::numeric_limits<::jxx::lang::jlong>::max()
                ? timeoutMillis + 1
                : timeoutMillis;
        (void)waitFor_(std::chrono::milliseconds(roundedMillis));
    }

    void Object::notify() {
        verifyMonitorOwner_();
        monitorCondition_.notify_one();
    }

    void Object::notifyAll() {
        verifyMonitorOwner_();
        monitorCondition_.notify_all();
    }

    jxx::Ptr<jxx::lang::Object> Object::cloneImpl() const {
        throw CloneNotSupportedException();
    }

    jxx::Ptr<jxx::lang::String> Object::getClassName_() const {
#if defined(__GNUG__) || defined(__clang__) || defined(_MSC_VER)
        return jxx::NEW<String>(demangle(typeid(*this).name()));
#else
        return jxx::NEW<String>("Object");
#endif
    }

}