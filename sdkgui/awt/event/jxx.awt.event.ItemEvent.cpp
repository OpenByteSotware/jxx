#include "awt/event/jxx.awt.event.ItemEvent.h"

#include <sstream>

namespace jxx::awt::event
{
    ItemEvent::ItemEvent(
        const ::jxx::Ptr<::jxx::awt::ItemSelectable>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::lang::Object>& item,
        ::jxx::lang::jint stateChange)
        : Super(::jxx::CAST<::jxx::lang::Object>(source), id), item_(item), stateChange_(stateChange)
    {
    }

    ::jxx::Ptr<::jxx::awt::ItemSelectable> ItemEvent::getItemSelectable() const
    {
        return ::jxx::CAST<::jxx::awt::ItemSelectable>(source);
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
