#pragma once
#include "awt/jxx.awt.Component.h"
namespace jxx::swing
{
    class Popup : public ::jxx::lang::ClassBase<Popup, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Popup, JxxSuper>;
        Popup();
        Popup(const ::jxx::Ptr<::jxx::awt::Component>& owner,
              const ::jxx::Ptr<::jxx::awt::Component>& contents,
              ::jxx::lang::jint x, ::jxx::lang::jint y);
        virtual void show();
        virtual void hide();
        ::jxx::lang::jbool isVisible() const noexcept;
    protected:
        ::jxx::Ptr<::jxx::awt::Component> owner_;
        ::jxx::Ptr<::jxx::awt::Component> contents_;
        ::jxx::lang::jint x_ = 0;
        ::jxx::lang::jint y_ = 0;
        ::jxx::lang::jbool visible_ = false;
    };
}
