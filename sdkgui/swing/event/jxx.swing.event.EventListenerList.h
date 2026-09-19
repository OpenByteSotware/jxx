#pragma once

#include <algorithm>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.EventListener.h"

namespace jxx::swing::event
{
    class EventListenerList : public ::jxx::lang::ClassBase<
        EventListenerList, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<EventListenerList, JxxSuper>;

        EventListenerList() = default;
        ~EventListenerList() override = default;

        template<typename Listener>
        void add(const ::jxx::Ptr<Listener>& listener)
        {
            static_assert(std::is_base_of_v<
                ::jxx::util::EventListener, Listener>);
            if (listener == nullptr)
                return;
            listeners_.push_back(
                ::jxx::CAST<::jxx::util::EventListener>(listener));
        }

        template<typename Listener>
        void remove(const ::jxx::Ptr<Listener>& listener)
        {
            static_assert(std::is_base_of_v<
                ::jxx::util::EventListener, Listener>);
            const auto target =
                ::jxx::CAST<::jxx::util::EventListener>(listener);
            const auto position = std::find(
                listeners_.rbegin(), listeners_.rend(), target);
            if (position != listeners_.rend())
                listeners_.erase(std::next(position).base());
        }

        template<typename Listener>
        ::jxx::lang::jint getListenerCount() const
        {
            static_assert(std::is_base_of_v<
                ::jxx::util::EventListener, Listener>);
            ::jxx::lang::jint count = 0;
            for (const auto& listener : listeners_)
            {
                if (::jxx::CAST<Listener>(listener) != nullptr)
                    ++count;
            }
            return count;
        }

        ::jxx::lang::jint getListenerCount() const noexcept;

        template<typename Listener>
        std::vector<::jxx::Ptr<Listener>> getListeners() const
        {
            static_assert(std::is_base_of_v<
                ::jxx::util::EventListener, Listener>);
            std::vector<::jxx::Ptr<Listener>> result;
            for (const auto& listener : listeners_)
            {
                const auto typed = ::jxx::CAST<Listener>(listener);
                if (typed != nullptr)
                    result.push_back(typed);
            }
            return result;
        }

    private:
        std::vector<::jxx::Ptr<::jxx::util::EventListener>> listeners_;
    };
}
