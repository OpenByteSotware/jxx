#include "swing/event/jxx.swing.event.HyperlinkEvent.h"

namespace jxx::swing::event
{
    HyperlinkEvent::EventType::EventType(
        const ::jxx::Ptr<::jxx::lang::String>& name)
        : Super(), name_(name) {}

    ::jxx::Ptr<::jxx::lang::String>
    HyperlinkEvent::EventType::toString() const
    {
        return name_;
    }

    ::jxx::Ptr<HyperlinkEvent::EventType>
        HyperlinkEvent::EventType::ENTERED =
            ::jxx::NEW<EventType>(
                ::jxx::NEW<::jxx::lang::String>("ENTERED"));
    ::jxx::Ptr<HyperlinkEvent::EventType>
        HyperlinkEvent::EventType::EXITED =
            ::jxx::NEW<EventType>(
                ::jxx::NEW<::jxx::lang::String>("EXITED"));
    ::jxx::Ptr<HyperlinkEvent::EventType>
        HyperlinkEvent::EventType::ACTIVATED =
            ::jxx::NEW<EventType>(
                ::jxx::NEW<::jxx::lang::String>("ACTIVATED"));

    HyperlinkEvent::HyperlinkEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<EventType>& type,
        const ::jxx::Ptr<::jxx::net::URL>& url)
        : HyperlinkEvent(source, type, url, nullptr) {}

    HyperlinkEvent::HyperlinkEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<EventType>& type,
        const ::jxx::Ptr<::jxx::net::URL>& url,
        const ::jxx::Ptr<::jxx::lang::String>& description)
        : Super(source), type_(type), url_(url), description_(description) {}

    ::jxx::Ptr<HyperlinkEvent::EventType>
    HyperlinkEvent::getEventType() const { return type_; }
    ::jxx::Ptr<::jxx::net::URL>
    HyperlinkEvent::getURL() const { return url_; }
    ::jxx::Ptr<::jxx::lang::String>
    HyperlinkEvent::getDescription() const { return description_; }
}
