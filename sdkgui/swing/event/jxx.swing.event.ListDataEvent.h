#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::event {
class ListDataEvent : public ::jxx::lang::ClassBase<ListDataEvent, ::jxx::util::EventObject> {
public:
    using JxxSuper = ::jxx::util::EventObject;
    using Super = ::jxx::lang::ClassBase<ListDataEvent, JxxSuper>;
    static constexpr ::jxx::lang::jint CONTENTS_CHANGED = 0;
    static constexpr ::jxx::lang::jint INTERVAL_ADDED = 1;
    static constexpr ::jxx::lang::jint INTERVAL_REMOVED = 2;
    ListDataEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint type,
        ::jxx::lang::jint index0, ::jxx::lang::jint index1);
    ::jxx::lang::jint getType() const;
    ::jxx::lang::jint getIndex0() const;
    ::jxx::lang::jint getIndex1() const;
private:
    ::jxx::lang::jint type_;
    ::jxx::lang::jint index0_;
    ::jxx::lang::jint index1_;
};
}
