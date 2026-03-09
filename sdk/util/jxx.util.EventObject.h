#pragma once
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <functional>
#include <vector>
#include <mutex>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include "lang/jxx.lang.Object.h"

namespace jxx::util {

// -----------------------------
// EventObject (Java-like base)
// -----------------------------
class EventObject {
public:
    explicit EventObject(const jxx::lang::Object* source)
        : source_(source)
    {
        if (!source_) throw std::invalid_argument("EventObject: source must not be nullptr");
    }

    virtual ~EventObject() = default;

    // Java-like: returns the event source (non-owning).
    const void* getSource() const noexcept { return source_; }

    // Optional pretty string (handy for logging)
    virtual std::string toString() const {
        std::ostringstream oss;
        oss << "EventObject[source=" << source_ << "]";
        return oss.str();
    }

private:
    const jxx::lang::Object* source_; // non-owning; source must outlive event dispatch
};

// -------------------------------------------------------
// TypedEvent<Source> – type-safe access to the source
// -------------------------------------------------------
template <typename Source>
class TypedEvent : public EventObject {
public:
    explicit TypedEvent(const Source* source)
        : EventObject(static_cast<const void*>(source)) {}

    const Source& getSourceTyped() const {
        return *static_cast<const Source*>(getSource());
    }
};

// -------------------------------------------------------
// EventDispatcher<E> – thread-safe listener management
// -------------------------------------------------------
template <typename E>
class EventDispatcher {
public:
    using Listener     = std::function<void(const E&)>;
    using ListenerId   = std::size_t;

    // RAII subscription handle (auto-unsubscribe on destruction)
    class Subscription {
    public:
        Subscription() = default;
        Subscription(EventDispatcher* d, ListenerId id) : disp_(d), id_(id) {}
        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;
        Subscription(Subscription&& other) noexcept { *this = std::move(other); }
        Subscription& operator=(Subscription&& other) noexcept {
            if (this != &other) {
                release();
                disp_ = other.disp_;
                id_   = other.id_;
                other.disp_ = nullptr;
                other.id_   = 0;
            }
            return *this;
        }
        ~Subscription() { release(); }
        void release() {
            if (disp_) {
                disp_->removeListener(id_);
                disp_ = nullptr;
                id_   = 0;
            }
        }
        explicit operator bool() const noexcept { return disp_ != nullptr; }
        ListenerId id() const noexcept { return id_; }
    private:
        EventDispatcher* disp_{nullptr};
        ListenerId id_{0};
    };

    EventDispatcher() = default;

    // Add listener: returns id (for manual removal) and RAII subscription (optional)
    ListenerId addListener(const Listener& listener) {
        std::lock_guard<std::mutex> lk(m_);
        const ListenerId id = nextId_++;
        listeners_.emplace_back(id, listener);
        return id;
    }

    Subscription addListenerScoped(const Listener& listener) {
        return Subscription(this, addListener(listener));
    }

    // Remove listener by id; returns true if removed
    bool removeListener(ListenerId id) {
        std::lock_guard<std::mutex> lk(m_);
        auto it = std::find_if(listeners_.begin(), listeners_.end(),
                               [=](const auto& p){ return p.first == id; });
        if (it == listeners_.end()) return false;
        listeners_.erase(it);
        return true;
    }

    // Fire event to a snapshot of listeners (no re-entrancy issues)
    void fire(const E& event) const {
        std::vector<Listener> snapshot;
        {
            std::lock_guard<std::mutex> lk(m_);
            snapshot.reserve(listeners_.size());
            for (const auto& p : listeners_) snapshot.push_back(p.second);
        }
        for (auto& cb : snapshot) cb(event);
    }

    // Clear all listeners
    void clear() {
        std::lock_guard<std::mutex> lk(m_);
        listeners_.clear();
    }

    std::size_t listenerCount() const {
        std::lock_guard<std::mutex> lk(m_);
        return listeners_.size();
    }

private:
    mutable std::mutex m_;
    std::vector<std::pair<ListenerId, Listener>> listeners_;
    ListenerId nextId_{1};
};

} // namespace jxx::event