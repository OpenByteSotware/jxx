#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::swing
{
    template<typename E>
    class ListModel : public ::jxx::lang::InterfaceBase<ListModel<E>>
    {
    public:
        ~ListModel() override = default;
        virtual ::jxx::lang::jint getSize() const = 0;
        virtual ::jxx::Ptr<E> getElementAt(::jxx::lang::jint index) const = 0;
    };
}
