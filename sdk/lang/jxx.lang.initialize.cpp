#include "lang/jxx.lang.initialize.h"

#include <algorithm>
#include <atomic>
#include <exception>
#include <utility>

namespace jxx::lang {
namespace class_info_detail {

std::vector<RegistrationFunction>& pendingRegistrations() {
    static std::vector<RegistrationFunction> registrations;
    return registrations;
}

std::mutex& pendingRegistrationMutex() {
    static std::mutex mutex;
    return mutex;
}

void addPendingRegistration(
    RegistrationFunction function) {

    if (function == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(
        pendingRegistrationMutex());

    auto& registrations =
        pendingRegistrations();

    if (std::find(
            registrations.begin(),
            registrations.end(),
            function) == registrations.end()) {

        registrations.push_back(function);
    }
}

std::size_t pendingRegistrationCount() {
    std::lock_guard<std::mutex> lock(
        pendingRegistrationMutex());

    return pendingRegistrations().size();
}

} // namespace class_info_detail

namespace {

std::atomic<bool>& initializedFlag() {
    static std::atomic<bool> initialized{ false };
    return initialized;
}

std::mutex& initializationMutex() {
    static std::mutex mutex;
    return mutex;
}

} // namespace

void initialize() {
    if (initializedFlag().load(
            std::memory_order_acquire)) {
        return;
    }

    std::lock_guard<std::mutex> initializationLock(
        initializationMutex());

    if (initializedFlag().load(
            std::memory_order_relaxed)) {
        return;
    }

    std::vector<
        class_info_detail::RegistrationFunction>
        registrations;

    {
        std::lock_guard<std::mutex> registrationLock(
            class_info_detail::pendingRegistrationMutex());

        registrations =
            class_info_detail::pendingRegistrations();
    }

    /*
     * Mark initialized only after all callbacks complete. If a callback throws,
     * a later call may retry the complete initialization sequence.
     */
    for (const auto registration : registrations) {
        if (registration != nullptr) {
            registration();
        }
    }

    initializedFlag().store(
        true,
        std::memory_order_release);
}

bool isInitialized() noexcept {
    return initializedFlag().load(
        std::memory_order_acquire);
}

} // namespace jxx::lang
