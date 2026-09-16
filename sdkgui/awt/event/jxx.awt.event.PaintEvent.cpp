#include "awt/event/jxx.awt.event.PaintEvent.h"

#include <sstream>

namespace jxx::awt::event
{
    PaintEvent::PaintEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::awt::Rectangle>& updateRect)
        : Super(source, id),
          updateRect_(updateRect)
    {
    }

    ::jxx::Ptr<::jxx::awt::Rectangle> PaintEvent::getUpdateRect() const
    {
        return updateRect_;
    }

    void PaintEvent::setUpdateRect(
        const ::jxx::Ptr<::jxx::awt::Rectangle>& updateRect)
    {
        updateRect_ = updateRect;
    }

    ::jxx::Ptr<::jxx::lang::String> PaintEvent::paramString() const
    {
        std::ostringstream out;
        out << (id == PAINT ? "PAINT" : id == UPDATE ? "UPDATE" : "unknown type")
            << ",updateRect=";
        if (updateRect_ != nullptr)
            out << updateRect_->toString()->utf8();
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
