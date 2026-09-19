#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::event
{
    class ListSelectionEvent : public ::jxx::lang::ClassBase<ListSelectionEvent,
        ::jxx::util::EventObject>
    {
    public:
        using JxxSuper = ::jxx::util::EventObject;
        using Super = ::jxx::lang::ClassBase<ListSelectionEvent, JxxSuper>;
        ListSelectionEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,
            ::jxx::lang::jint firstIndex, ::jxx::lang::jint lastIndex,
            ::jxx::lang::jbool isAdjusting);
        ::jxx::lang::jint getFirstIndex() const;
        ::jxx::lang::jint getLastIndex() const;
        ::jxx::lang::jbool getValueIsAdjusting() const;
    private:
        ::jxx::lang::jint firstIndex_;
        ::jxx::lang::jint lastIndex_;
        ::jxx::lang::jbool isAdjusting_;
    };
}
