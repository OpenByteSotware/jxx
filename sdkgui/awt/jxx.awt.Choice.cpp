#include "awt/jxx.awt.Choice.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.ItemEvent.h"
#include "awt/event/jxx.awt.event.ItemListener.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt
{
    Choice::Choice() : selectedIndex_(-1) {}

    ::jxx::lang::jint Choice::getItemCount() const
    {
        return static_cast<::jxx::lang::jint>(items_.size());
    }

    ::jxx::lang::jint Choice::countItems() const { return getItemCount(); }

    ::jxx::Ptr<::jxx::lang::String> Choice::getItem(
        ::jxx::lang::jint index) const
    {
        if (index < 0 || index >= getItemCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        return items_[static_cast<std::size_t>(index)];
    }

    void Choice::add(const ::jxx::Ptr<::jxx::lang::String>& item)
    {
        insert(item, getItemCount());
    }

    void Choice::addItem(const ::jxx::Ptr<::jxx::lang::String>& item)
    {
        add(item);
    }

    void Choice::insert(const ::jxx::Ptr<::jxx::lang::String>& item,
        ::jxx::lang::jint index)
    {
        if (item == nullptr) throw ::jxx::lang::NullPointerException("item");
        if (index < 0 || index > getItemCount())
            throw ::jxx::lang::IllegalArgumentException("index");
        items_.insert(items_.begin() + index, item);
        if (selectedIndex_ < 0) selectedIndex_ = 0;
        else if (index <= selectedIndex_) ++selectedIndex_;
        invalidate();
    }

    void Choice::remove(const ::jxx::Ptr<::jxx::lang::String>& item)
    {
        if (item == nullptr) throw ::jxx::lang::NullPointerException("item");
        for (::jxx::lang::jint index = 0; index < getItemCount(); ++index)
        {
            if (items_[static_cast<std::size_t>(index)]->equals(item))
            {
                remove(index);
                return;
            }
        }
        throw ::jxx::lang::IllegalArgumentException("item");
    }

    void Choice::remove(::jxx::lang::jint index)
    {
        if (index < 0 || index >= getItemCount())
            throw ::jxx::lang::IndexOutOfBoundsException("index");
        items_.erase(items_.begin() + index);
        if (items_.empty()) selectedIndex_ = -1;
        else if (selectedIndex_ == index)
            selectedIndex_ = 0;
        else if (selectedIndex_ > index)
            --selectedIndex_;
        invalidate();
    }

    void Choice::removeAll()
    {
        items_.clear();
        selectedIndex_ = -1;
        invalidate();
    }

    ::jxx::Ptr<::jxx::lang::String> Choice::getSelectedItem() const
    {
        return selectedIndex_ < 0 ? nullptr : getItem(selectedIndex_);
    }

    ::jxx::lang::ObjectArray Choice::getSelectedObjects() const
    {
        const auto selected = getSelectedItem();
        if (selected == nullptr) return nullptr;
        auto result = ::jxx::NEW<::jxx::lang::ObjectArrayType>(1);
        (*result)[0] = ::jxx::CAST<::jxx::lang::Object>(selected);
        return result;
    }

    ::jxx::lang::jint Choice::getSelectedIndex() const { return selectedIndex_; }

    void Choice::select(::jxx::lang::jint index)
    {
        if (index < 0 || index >= getItemCount())
            throw ::jxx::lang::IllegalArgumentException("index");
        setSelectedIndex(index, false);
    }

    void Choice::select(const ::jxx::Ptr<::jxx::lang::String>& item)
    {
        if (item == nullptr) return;
        for (::jxx::lang::jint index = 0; index < getItemCount(); ++index)
            if (items_[static_cast<std::size_t>(index)]->equals(item))
            {
                setSelectedIndex(index, false);
                return;
            }
    }

    void Choice::addItemListener(
        const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener)
    {
        if (listener != nullptr &&
            std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end())
            listeners_.push_back(listener);
    }

    void Choice::removeItemListener(
        const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener)
    {
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void Choice::setSelectedIndex(::jxx::lang::jint index,
        ::jxx::lang::jbool fireEvent)
    {
        if (selectedIndex_ == index) return;
        selectedIndex_ = index;
        if (fireEvent)
            processItemEvent(::jxx::NEW<::jxx::awt::event::ItemEvent>(
                ::jxx::CAST<ItemSelectable>(thisPtr()),
                ::jxx::awt::event::ItemEvent::ITEM_STATE_CHANGED,
                ::jxx::CAST<::jxx::lang::Object>(getSelectedItem()),
                ::jxx::awt::event::ItemEvent::SELECTED));
    }

    void Choice::processItemEvent(
        const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event)
    {
        const auto listeners = listeners_;
        for (const auto& listener : listeners)
            if (listener != nullptr) listener->itemStateChanged(event);
    }
}
