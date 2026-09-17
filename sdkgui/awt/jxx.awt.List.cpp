#include "awt/jxx.awt.List.h"

#include <algorithm>
#include <sstream>

#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "awt/event/jxx.awt.event.ItemEvent.h"
#include "awt/event/jxx.awt.event.ItemListener.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt
{
    List::List() : List(4, false) {}
    List::List(::jxx::lang::jint rows) : List(rows, false) {}
    List::List(::jxx::lang::jint rows, ::jxx::lang::jbool multipleMode)
        : rows_(rows > 0 ? rows : 4), multipleMode_(multipleMode),
          visibleIndex_(-1) {}

    ::jxx::lang::jint List::getItemCount() const
    { return static_cast<::jxx::lang::jint>(items_.size()); }
    ::jxx::lang::jint List::countItems() const { return getItemCount(); }
    ::jxx::Ptr<::jxx::lang::String> List::getItem(::jxx::lang::jint index) const
    {
        if (index < 0 || index >= getItemCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        return items_[static_cast<std::size_t>(index)];
    }
    void List::add(const ::jxx::Ptr<::jxx::lang::String>& item)
    { add(item, -1); }
    void List::add(const ::jxx::Ptr<::jxx::lang::String>& item,
        ::jxx::lang::jint index)
    {
        if (item == nullptr) throw ::jxx::lang::NullPointerException("item");
        if (index < -1 || index > getItemCount())
            throw ::jxx::lang::IllegalArgumentException("index");
        if (index == -1) index = getItemCount();
        items_.insert(items_.begin() + index, item);
        for (auto& selected : selectedIndexes_) if (selected >= index) ++selected;
        invalidate();
    }
    void List::addItem(const ::jxx::Ptr<::jxx::lang::String>& item) { add(item); }
    void List::addItem(const ::jxx::Ptr<::jxx::lang::String>& item,
        ::jxx::lang::jint index) { add(item, index); }
    void List::replaceItem(const ::jxx::Ptr<::jxx::lang::String>& item,
        ::jxx::lang::jint index)
    {
        if (item == nullptr) throw ::jxx::lang::NullPointerException("item");
        if (index < 0 || index >= getItemCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        items_[static_cast<std::size_t>(index)] = item;
    }
    void List::removeAll()
    { items_.clear(); selectedIndexes_.clear(); visibleIndex_ = -1; invalidate(); }
    void List::clear() { removeAll(); }
    void List::remove(const ::jxx::Ptr<::jxx::lang::String>& item)
    {
        if (item == nullptr) throw ::jxx::lang::NullPointerException("item");
        for (::jxx::lang::jint i = 0; i < getItemCount(); ++i)
            if (items_[static_cast<std::size_t>(i)]->equals(item)) { remove(i); return; }
        throw ::jxx::lang::IllegalArgumentException("item");
    }
    void List::remove(::jxx::lang::jint index)
    {
        if (index < 0 || index >= getItemCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        items_.erase(items_.begin() + index);
        selectedIndexes_.erase(std::remove(selectedIndexes_.begin(),
            selectedIndexes_.end(), index), selectedIndexes_.end());
        for (auto& selected : selectedIndexes_) if (selected > index) --selected;
        if (visibleIndex_ == index) visibleIndex_ = -1;
        else if (visibleIndex_ > index) --visibleIndex_;
        invalidate();
    }
    void List::delItem(::jxx::lang::jint index) { remove(index); }
    ::jxx::lang::jint List::getSelectedIndex() const
    { return selectedIndexes_.size() == 1 ? selectedIndexes_.front() : -1; }
    ::jxx::Ptr<::jxx::lang::String> List::getSelectedItem() const
    { const auto i = getSelectedIndex(); return i < 0 ? nullptr : getItem(i); }
    ::jxx::lang::ObjectArray List::getSelectedObjects() const
    {
        if (selectedIndexes_.empty()) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::ObjectArrayType>(
            static_cast<::jxx::lang::jint>(selectedIndexes_.size()));
        for (std::size_t i = 0; i < selectedIndexes_.size(); ++i)
            (*result)[static_cast<::jxx::lang::jint>(i)] =
                ::jxx::CAST<::jxx::lang::Object>(getItem(selectedIndexes_[i]));
        return result;
    }
    void List::select(::jxx::lang::jint index)
    {
        if (index < 0 || index >= getItemCount())
            throw ::jxx::lang::IllegalArgumentException("index");
        if (!multipleMode_) selectedIndexes_.clear();
        if (!isIndexSelected(index)) selectedIndexes_.push_back(index);
        std::sort(selectedIndexes_.begin(), selectedIndexes_.end());
    }
    void List::deselect(::jxx::lang::jint index)
    { selectedIndexes_.erase(std::remove(selectedIndexes_.begin(), selectedIndexes_.end(), index), selectedIndexes_.end()); }
    ::jxx::lang::jbool List::isIndexSelected(::jxx::lang::jint index) const
    { return std::find(selectedIndexes_.begin(), selectedIndexes_.end(), index) != selectedIndexes_.end(); }
    ::jxx::lang::jbool List::isSelected(::jxx::lang::jint index) const
    { return isIndexSelected(index); }
    ::jxx::lang::jint List::getRows() const { return rows_; }
    ::jxx::lang::jbool List::isMultipleMode() const { return multipleMode_; }
    ::jxx::lang::jbool List::allowsMultipleSelections() const { return isMultipleMode(); }
    void List::setMultipleMode(::jxx::lang::jbool value)
    { multipleMode_ = value; if (!value && selectedIndexes_.size() > 1) selectedIndexes_.erase(selectedIndexes_.begin() + 1, selectedIndexes_.end()); }
    void List::setMultipleSelections(::jxx::lang::jbool value) { setMultipleMode(value); }
    ::jxx::lang::jint List::getVisibleIndex() const { return visibleIndex_; }
    void List::makeVisible(::jxx::lang::jint index)
    { if (index >= 0 && index < getItemCount()) visibleIndex_ = index; }
    ::jxx::Ptr<Dimension> List::getPreferredSize(::jxx::lang::jint rows) const
    { return ::jxx::NEW<Dimension>(100, std::max(1, rows) * 16 + 4); }
    ::jxx::Ptr<Dimension> List::preferredSize(::jxx::lang::jint rows) const
    { return getPreferredSize(rows); }
    ::jxx::Ptr<Dimension> List::getMinimumSize(::jxx::lang::jint rows) const
    { return getPreferredSize(rows); }
    ::jxx::Ptr<Dimension> List::minimumSize(::jxx::lang::jint rows) const
    { return getMinimumSize(rows); }
    void List::addItemListener(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& l)
    { if (l && std::find(itemListeners_.begin(), itemListeners_.end(), l) == itemListeners_.end()) itemListeners_.push_back(l); }
    void List::removeItemListener(const ::jxx::Ptr<::jxx::awt::event::ItemListener>& l)
    { itemListeners_.erase(std::remove(itemListeners_.begin(), itemListeners_.end(), l), itemListeners_.end()); }
    void List::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& l)
    { if (l && std::find(actionListeners_.begin(), actionListeners_.end(), l) == actionListeners_.end()) actionListeners_.push_back(l); }
    void List::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& l)
    { actionListeners_.erase(std::remove(actionListeners_.begin(), actionListeners_.end(), l), actionListeners_.end()); }
    void List::setSelectedFromNative(::jxx::lang::jint index, ::jxx::lang::jbool selected)
    {
        const auto previous = isIndexSelected(index);
        if (selected) select(index); else deselect(index);
        if (previous == selected) return;
        processItemEvent(::jxx::NEW<::jxx::awt::event::ItemEvent>(
            ::jxx::CAST<ItemSelectable>(thisPtr()),
            ::jxx::awt::event::ItemEvent::ITEM_STATE_CHANGED,
            ::jxx::CAST<::jxx::lang::Object>(getItem(index)),
            selected ? ::jxx::awt::event::ItemEvent::SELECTED : ::jxx::awt::event::ItemEvent::DESELECTED));
    }
    void List::fireActionPerformed(::jxx::lang::jint index)
    {
        if (index < 0 || index >= getItemCount()) return;
        processActionEvent(::jxx::NEW<::jxx::awt::event::ActionEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()),
            ::jxx::awt::event::ActionEvent::ACTION_PERFORMED, getItem(index)));
    }
    void List::processItemEvent(const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& e)
    { const auto copy = itemListeners_; for (const auto& l : copy) if (l) l->itemStateChanged(e); }
    void List::processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& e)
    { const auto copy = actionListeners_; for (const auto& l : copy) if (l) l->actionPerformed(e); }
    ::jxx::Ptr<::jxx::lang::String> List::paramString() const
    {
        std::ostringstream out;
        out << "selected=" << getSelectedIndex() << ",rows=" << rows_
            << ",multiple=" << (multipleMode_ ? "true" : "false");
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
