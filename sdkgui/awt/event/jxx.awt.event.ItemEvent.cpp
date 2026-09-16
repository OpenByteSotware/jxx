#include "awt/event/jxx.awt.event.ItemEvent.h"

#include <sstream>

namespace jxx::awt::event
{
    ItemEvent::ItemEvent(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::lang::Object>& item,
        ::jxx::lang::jint stateChange)
        : Super(source, id), item_(item), stateChange_(stateChange)
    {
    }

    ::jxx::Ptr<::jxx::lang::Object> ItemEvent::getItemSelectable() const
    {
        return source;
    }

    ::jxx::Ptr<::jxx::lang::Object> ItemEvent::getItem() const
    {
        return item_;
    }

    ::jxx::lang::jint ItemEvent::getStateChange() const
    {
        return stateChange_;
    }

    ::jxx::Ptr<::jxx::lang::String> ItemEvent::paramString() const
    {
        std::ostringstream out;
        out << "ITEM_STATE_CHANGED,item=";
        if (item_ != nullptr) out << item_->toString()->utf8();
        out << ",stateChange="
            << (stateChange_ == SELECTED ? "SELECTED" : "DESELECTED");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
