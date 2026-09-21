#include "swing/jxx.swing.MenuSelectionManager.h"

#include <algorithm>

#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "awt/event/jxx.awt.event.MouseEvent.h"
#include "swing/event/jxx.swing.event.ChangeEvent.h"
#include "swing/event/jxx.swing.event.ChangeListener.h"

namespace jxx::swing
{
    ::jxx::Ptr<MenuSelectionManager> MenuSelectionManager::defaultManager()
    {
        static auto manager = ::jxx::NEW<MenuSelectionManager>();
        return manager;
    }

    void MenuSelectionManager::setSelectedPath(
        const MenuElementArray& path)
    {
        std::vector<::jxx::Ptr<MenuElement>> next;
        if (path != nullptr)
        {
            next.reserve(path->length);
            for (::jxx::lang::jint index = 0;
                 index < static_cast<::jxx::lang::jint>(path->length);
                 ++index)
            {
                const auto element = (*path)[index];
                if (element != nullptr)
                    next.push_back(element);
            }
        }

        std::size_t common = 0;
        while (common < selectedPath_.size() &&
               common < next.size() &&
               selectedPath_[common] == next[common])
        {
            ++common;
        }

        for (std::size_t index = selectedPath_.size(); index > common; --index)
        {
            selectedPath_[index - 1]->menuSelectionChanged(false);
        }

        for (std::size_t index = common; index < next.size(); ++index)
        {
            next[index]->menuSelectionChanged(true);
        }

        selectedPath_ = std::move(next);
        fireStateChanged();
    }

    MenuSelectionManager::MenuElementArray
    MenuSelectionManager::getSelectedPath() const
    {
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<MenuElement>, 1>>(
                static_cast<std::uint32_t>(selectedPath_.size()));

        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = selectedPath_[index];

        return result;
    }

    void MenuSelectionManager::clearSelectedPath()
    {
        setSelectedPath(nullptr);
    }

    void MenuSelectionManager::processMouseEvent(
        const ::jxx::Ptr<::jxx::awt::event::MouseEvent>& event)
    {
        if (selectedPath_.empty()) return;
        auto path = getSelectedPath();
        selectedPath_.back()->processMouseEvent(
            event, path, ::jxx::CAST<MenuSelectionManager>(thisPtr()));
    }

    void MenuSelectionManager::processKeyEvent(
        const ::jxx::Ptr<::jxx::awt::event::KeyEvent>& event)
    {
        if (selectedPath_.empty()) return;
        auto path = getSelectedPath();
        selectedPath_.back()->processKeyEvent(
            event, path, ::jxx::CAST<MenuSelectionManager>(thisPtr()));
    }

    ::jxx::Ptr<::jxx::awt::Component>
    MenuSelectionManager::componentForPoint(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        const ::jxx::Ptr<::jxx::awt::Point>&) const
    {
        return isComponentPartOfCurrentMenu(source) ? source : nullptr;
    }

    ::jxx::lang::jbool
    MenuSelectionManager::isComponentPartOfCurrentMenu(
        const ::jxx::Ptr<::jxx::awt::Component>& component) const
    {
        if (component == nullptr) return false;
        for (const auto& element : selectedPath_)
        {
            if (element != nullptr && element->getComponent() == component)
                return true;
        }
        return false;
    }

    void MenuSelectionManager::addChangeListener(
        const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    {
        if (listener != nullptr)
            changeListeners_.push_back(listener);
    }

    void MenuSelectionManager::removeChangeListener(
        const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener)
    {
        changeListeners_.erase(
            std::remove(changeListeners_.begin(),
                changeListeners_.end(), listener),
            changeListeners_.end());
    }

    void MenuSelectionManager::fireStateChanged()
    {
        auto event = ::jxx::NEW<::jxx::swing::event::ChangeEvent>(
            ::jxx::CAST<::jxx::lang::Object>(thisPtr()));
        const auto copy = changeListeners_;
        for (const auto& listener : copy)
            if (listener != nullptr) listener->stateChanged(event);
    }
}
