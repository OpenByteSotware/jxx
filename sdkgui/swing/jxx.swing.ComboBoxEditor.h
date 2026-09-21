#pragma once
#include "awt/jxx.awt.Component.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::swing
{
    class ComboBoxEditor : public ::jxx::lang::InterfaceBase<ComboBoxEditor>
    {
    public:
        ~ComboBoxEditor() override = default;

        virtual ::jxx::Ptr<::jxx::awt::Component> getEditorComponent() const = 0;
        virtual void setItem(const ::jxx::Ptr<::jxx::lang::Object>& item) = 0;
        virtual ::jxx::Ptr<::jxx::lang::Object> getItem() const = 0;
        virtual void selectAll() = 0;
        virtual void addActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener) = 0;
        virtual void removeActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener) = 0;
    };
}
