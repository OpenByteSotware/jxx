#pragma once

#include <cstddef>
#include <functional>
#include <algorithm>
#include <mutex>
#include <utility>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util {

class EventObject
    : public ::jxx::lang::ClassBase<
          EventObject,
          ::jxx::lang::Object> {
public:
    explicit EventObject(
        const ::jxx::Ptr<::jxx::lang::Object>& source);

    ~EventObject() override;

    ::jxx::Ptr<::jxx::lang::Object> getSource() const;

    ::jxx::Ptr<::jxx::lang::String> toString() const override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;

private:
    ::jxx::Ptr<::jxx::lang::Object> source_;
};

template <typename Source>
class TypedEvent
    : public EventObject {
public:
    explicit TypedEvent(
        const ::jxx::Ptr<Source>& source)
        : EventObject(
              ::jxx::CAST<::jxx::lang::Object>(source)) {
    }

    ::jxx::Ptr<Source> getSourceTyped() const {
        return ::jxx::CAST<Source>(getSource());
    }
};

template <typename E>
class EventDispatcher {
public:
    using Listener = std::function<void(const E&)>;
    using ListenerId = std::size_t;

    class Subscription {
    public:
        Subscription() = default;

        Subscription(
            EventDispatcher* dispatcher,
            ListenerId id)
            : dispatcher_(dispatcher)
            , id_(id) {
        }

        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;

        Subscription(Subscription&& other) noexcept
            : dispatcher_(other.dispatcher_)
            , id_(other.id_) {
            other.dispatcher_ = nullptr;
        }

        Subscription& operator=(Subscription&& other) noexcept {
            if (this != &other) {
                reset();
                dispatcher_ = other.dispatcher_;
                id_ = other.id_;
                other.dispatcher_ = nullptr;
            }
            return *this;
        }

        ~Subscription() {
            reset();
        }

        void reset() {
            if (dispatcher_ != nullptr) {
                dispatcher_->removeListener(id_);
                dispatcher_ = nullptr;
            }
        }

    private:
        EventDispatcher* dispatcher_ = nullptr;
        ListenerId id_ = 0U;
    };

    Subscription addListener(Listener listener) {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto id = nextId_++;
        listeners_.emplace_back(id, std::move(listener));
        return Subscription(this, id);
    }

    void removeListener(ListenerId id) {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners_.erase(
            std::remove_if(
                listeners_.begin(),
                listeners_.end(),
                [id](const auto& entry) {
                    return entry.first == id;
                }),
            listeners_.end());
    }

    void dispatch(const E& event) const {
        std::vector<Listener> snapshot;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            snapshot.reserve(listeners_.size());
            for (const auto& entry : listeners_) {
                snapshot.push_back(entry.second);
            }
        }
        for (const auto& listener : snapshot) {
            listener(event);
        }
    }

private:
    mutable std::mutex mutex_;
    std::vector<std::pair<ListenerId, Listener>> listeners_;
    ListenerId nextId_ = 1U;
};

} // namespace jxx::util
