#pragma once

#include <cstddef>
#include <mutex>
#include <type_traits>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.ClassInfoMarker.h"

namespace jxx::lang {
namespace class_info_detail {

/** Function signature stored by the deferred class-registration system. */
using RegistrationFunction = void (*)();

/**
 * Returns the process-wide pending registration list.
 *
 * A function-local static avoids dependencies on cross-translation-unit
 * initialization order.
 */
std::vector<RegistrationFunction>& pendingRegistrations();

/** Mutex protecting additions and snapshots of the pending list. */
std::mutex& pendingRegistrationMutex();

/**
 * Adds a callback if it is non-null and has not already been added.
 *
 * Duplicate suppression is useful when an inline registration declaration is
 * visible in multiple translation units.
 */
void addPendingRegistration(RegistrationFunction function);

/** Returns the current number of unique pending callbacks. */
std::size_t pendingRegistrationCount();

/**
 * Calls the exact ClassInfo specialization exposed by T.
 */
template <typename T>
void initializeRegisteredType() {
    static_assert(
        HasClassInfo<T>::value,
        "JXX_CLASS_REGISTER(T) requires T to inherit ClassBase, "
        "InterfaceBase, or expose JxxClassInfoMarker.");

    using ExactClassInfo =
        typename T::JxxClassInfoMarker;

    ExactClassInfo::initializeClass();
}

/**
 * Lightweight pre-main object which adds one function pointer to the pending
 * list. It does not create ClassAny metadata itself.
 */
template <typename T>
class PendingTypeRegistration final {
public:
    PendingTypeRegistration() {
        addPendingRegistration(
            &initializeRegisteredType<T>);
    }
};

} // namespace class_info_detail

/**
 * Registers all callbacks accumulated through JXX_CLASS_REGISTER(T).
 *
 * The operation is idempotent. Repeated calls are safe and do not recreate
 * already canonical ClassAny descriptors.
 */
void initialize();

/** Returns true after initialize() has completed successfully at least once. */
bool isInitialized() noexcept;

} // namespace jxx::lang

#ifndef JXX_DETAIL_CAT_IMPL
#define JXX_DETAIL_CAT_IMPL(a, b) a##b
#endif

#ifndef JXX_DETAIL_CAT
#define JXX_DETAIL_CAT(a, b) JXX_DETAIL_CAT_IMPL(a, b)
#endif

/**
 * Adds T's ClassInfo initialization callback to the deferred registration
 * list. Place this after T's complete class definition in a header or source
 * file.
 *
 * Example:
 *
 *   class Device final : public jxx::lang::ClassBase<Device> { ... };
 *   JXX_CLASS_REGISTER(jxx::model::Device)
 */
#define JXX_CLASS_REGISTER(T)                                                   \
namespace {                                                                     \
    [[maybe_unused]] inline const                                               \
        ::jxx::lang::class_info_detail::PendingTypeRegistration<T>             \
    JXX_DETAIL_CAT(jxx_pending_class_registration_, __COUNTER__){};             \
}
