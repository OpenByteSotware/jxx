#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::swing::event { class ChangeListener; }
namespace jxx::swing
{
    class SingleSelectionModel : public ::jxx::lang::InterfaceBase<SingleSelectionModel>
    {
    public:
        ~SingleSelectionModel() override = default;
        virtual ::jxx::lang::jint getSelectedIndex() const = 0;
        virtual void setSelectedIndex(::jxx::lang::jint index) = 0;
        virtual void clearSelection() = 0;
        virtual ::jxx::lang::jbool isSelected() const = 0;
        virtual void addChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
        virtual void removeChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
    };
}
