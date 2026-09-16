#pragma once

#include <vector>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.ItemSelectable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt::event
{
    class ItemEvent;
    class ItemListener;
}

namespace jxx::awt
{
    class Choice :
        public ::jxx::lang::ClassBase<Choice, Component, ItemSelectable>
    {
    public:
        using JxxSuper = Component;
        using Super = ::jxx::lang::ClassBase<Choice,
            JxxSuper, ItemSelectable>;

        Choice();
        ~Choice() override = default;

        ::jxx::lang::jint getItemCount() const;
        ::jxx::lang::jint countItems() const;
        ::jxx::Ptr<::jxx::lang::String> getItem(::jxx::lang::jint index) const;
        void add(const ::jxx::Ptr<::jxx::lang::String>& item);
        void addItem(const ::jxx::Ptr<::jxx::lang::String>& item);
        void insert(const ::jxx::Ptr<::jxx::lang::String>& item,
            ::jxx::lang::jint index);
        void remove(const ::jxx::Ptr<::jxx::lang::String>& item);
        void remove(::jxx::lang::jint index);
        void removeAll();

        ::jxx::Ptr<::jxx::lang::String> getSelectedItem() const;
        ::jxx::lang::ObjectArray getSelectedObjects() const override;
        ::jxx::lang::jint getSelectedIndex() const;
        void select(::jxx::lang::jint index);
        void select(const ::jxx::Ptr<::jxx::lang::String>& item);

        void addItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
        void removeItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;

    protected:
        virtual void processItemEvent(
            const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event);

    private:
        void setSelectedIndex(::jxx::lang::jint index,
            ::jxx::lang::jbool fireEvent);

        std::vector<::jxx::Ptr<::jxx::lang::String>> items_;
        ::jxx::lang::jint selectedIndex_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ItemListener>> listeners_;
    };
}
