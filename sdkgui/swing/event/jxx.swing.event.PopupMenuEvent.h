#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::event {
class PopupMenuEvent : public ::jxx::lang::ClassBase<PopupMenuEvent, ::jxx::util::EventObject> {
public:
    using JxxSuper = ::jxx::util::EventObject;
    using Super = ::jxx::lang::ClassBase<PopupMenuEvent, JxxSuper>;
    explicit PopupMenuEvent(const ::jxx::Ptr<::jxx::lang::Object>& source);
};
}
