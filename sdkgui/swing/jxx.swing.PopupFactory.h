#pragma once
#include "swing/jxx.swing.Popup.h"
namespace jxx::swing
{
    class PopupFactory : public ::jxx::lang::ClassBase<PopupFactory, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<PopupFactory, JxxSuper>;
        static void setSharedInstance(const ::jxx::Ptr<PopupFactory>& factory);
        static ::jxx::Ptr<PopupFactory> getSharedInstance();
        virtual ::jxx::Ptr<Popup> getPopup(
            const ::jxx::Ptr<::jxx::awt::Component>& owner,
            const ::jxx::Ptr<::jxx::awt::Component>& contents,
            ::jxx::lang::jint x, ::jxx::lang::jint y);
    };
}
