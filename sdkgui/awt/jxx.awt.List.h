#pragma once

#include <vector>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.ItemSelectable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt::event
{
    class ActionEvent;
    class ActionListener;
    class ItemEvent;
    class ItemListener;
}

namespace jxx::awt
{
    class List : public ::jxx::lang::ClassBase<List, Component, ItemSelectable>
    {
    public:
        using JxxSuper = Component;
        using Super = ::jxx::lang::ClassBase<List, JxxSuper, ItemSelectable>;

        List();
        explicit List(::jxx::lang::jint rows);
        List(::jxx::lang::jint rows, ::jxx::lang::jbool multipleMode);
        ~List() override = default;

        ::jxx::lang::jint getItemCount() const;
        ::jxx::lang::jint countItems() const;
        ::jxx::Ptr<::jxx::lang::String> getItem(::jxx::lang::jint index) const;
        void add(const ::jxx::Ptr<::jxx::lang::String>& item);
        void add(const ::jxx::Ptr<::jxx::lang::String>& item,
            ::jxx::lang::jint index);
        void addItem(const ::jxx::Ptr<::jxx::lang::String>& item);
        void addItem(const ::jxx::Ptr<::jxx::lang::String>& item,
            ::jxx::lang::jint index);
        void replaceItem(const ::jxx::Ptr<::jxx::lang::String>& item,
            ::jxx::lang::jint index);
        void removeAll();
        void clear();
        void remove(const ::jxx::Ptr<::jxx::lang::String>& item);
        void remove(::jxx::lang::jint index);
        void delItem(::jxx::lang::jint index);
        ::jxx::lang::jint getSelectedIndex() const;
        ::jxx::Ptr<::jxx::lang::String> getSelectedItem() const;
        ::jxx::lang::ObjectArray getSelectedObjects() const override;
        void select(::jxx::lang::jint index);
        void deselect(::jxx::lang::jint index);
        ::jxx::lang::jbool isIndexSelected(::jxx::lang::jint index) const;
        ::jxx::lang::jbool isSelected(::jxx::lang::jint index) const;
        ::jxx::lang::jint getRows() const;
        ::jxx::lang::jbool isMultipleMode() const;
        ::jxx::lang::jbool allowsMultipleSelections() const;
        void setMultipleMode(::jxx::lang::jbool multipleMode);
        void setMultipleSelections(::jxx::lang::jbool multipleMode);
        ::jxx::lang::jint getVisibleIndex() const;
        void makeVisible(::jxx::lang::jint index);
        ::jxx::Ptr<Dimension> getPreferredSize(::jxx::lang::jint rows) const;
        ::jxx::Ptr<Dimension> preferredSize(::jxx::lang::jint rows) const;
        ::jxx::Ptr<Dimension> getMinimumSize(::jxx::lang::jint rows) const;
        ::jxx::Ptr<Dimension> minimumSize(::jxx::lang::jint rows) const;
        void addItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
        void removeItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
        void addActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
        void removeActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);

    protected:
        virtual void processItemEvent(
            const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event);
        virtual void processActionEvent(
            const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event);
        ::jxx::Ptr<::jxx::lang::String> paramString() const;

    private:
        void setSelectedFromNative(::jxx::lang::jint index,
            ::jxx::lang::jbool selected);
        void fireActionPerformed(::jxx::lang::jint index);

        ::jxx::lang::jint rows_;
        ::jxx::lang::jbool multipleMode_;
        ::jxx::lang::jint visibleIndex_;
        std::vector<::jxx::Ptr<::jxx::lang::String>> items_;
        std::vector<::jxx::lang::jint> selectedIndexes_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ItemListener>> itemListeners_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> actionListeners_;
    };
}
