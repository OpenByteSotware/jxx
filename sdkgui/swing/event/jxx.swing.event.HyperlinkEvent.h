#pragma once

#include "util/jxx.util.EventObject.h"

namespace jxx::net { class URL; }

namespace jxx::swing::event
{
    class HyperlinkEvent : public ::jxx::lang::ClassBase<
        HyperlinkEvent, ::jxx::util::EventObject>
    {
    public:
        class EventType : public ::jxx::lang::ClassBase<
            EventType, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<EventType, JxxSuper>;

            explicit EventType(
                const ::jxx::Ptr<::jxx::lang::String>& name);
            ::jxx::Ptr<::jxx::lang::String> toString() const override;

            static ::jxx::Ptr<EventType> ENTERED;
            static ::jxx::Ptr<EventType> EXITED;
            static ::jxx::Ptr<EventType> ACTIVATED;

        private:
            ::jxx::Ptr<::jxx::lang::String> name_;
        };

        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<HyperlinkEvent, JxxSuper>;

        HyperlinkEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,
            const ::jxx::Ptr<EventType>& type,
            const ::jxx::Ptr<::jxx::net::URL>& url);
        HyperlinkEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,
            const ::jxx::Ptr<EventType>& type,
            const ::jxx::Ptr<::jxx::net::URL>& url,
            const ::jxx::Ptr<::jxx::lang::String>& description);

        ::jxx::Ptr<EventType> getEventType() const;
        ::jxx::Ptr<::jxx::net::URL> getURL() const;
        ::jxx::Ptr<::jxx::lang::String> getDescription() const;

    private:
        ::jxx::Ptr<EventType> type_;
        ::jxx::Ptr<::jxx::net::URL> url_;
        ::jxx::Ptr<::jxx::lang::String> description_;
    };
}
