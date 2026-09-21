#include "swing/jxx.swing.RepaintManager.h"

#include <algorithm>
#include <mutex>

#include "awt/jxx.awt.Component.h"
#include "swing/jxx.swing.JComponent.h"

namespace jxx::swing
{
    namespace
    {
        std::mutex& managerMutex()
        {
            static std::mutex value;
            return value;
        }

        ::jxx::Ptr<RepaintManager>& managerInstance()
        {
            static ::jxx::Ptr<RepaintManager> value;
            return value;
        }
    }

    RepaintManager::RepaintManager()
        : Super(),
          doubleBufferMaximumSize_(
              ::jxx::NEW<::jxx::awt::Dimension>(4096, 4096))
    {
    }

    ::jxx::Ptr<RepaintManager> RepaintManager::currentManager(
        const ::jxx::Ptr<::jxx::awt::Component>&)
    {
        std::lock_guard<std::mutex> lock(managerMutex());
        auto& value = managerInstance();
        if (value == nullptr) value = ::jxx::NEW<RepaintManager>();
        return value;
    }

    ::jxx::Ptr<RepaintManager> RepaintManager::currentManager(
        const ::jxx::Ptr<JComponent>& component)
    {
        return currentManager(
            ::jxx::CAST<::jxx::awt::Component>(component));
    }

    void RepaintManager::setCurrentManager(
        const ::jxx::Ptr<RepaintManager>& manager)
    {
        std::lock_guard<std::mutex> lock(managerMutex());
        managerInstance() = manager;
    }

    void RepaintManager::addDirtyRegion(
        const ::jxx::Ptr<JComponent>& component,
        ::jxx::lang::jint x,
        ::jxx::lang::jint y,
        ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        if (component == nullptr || width <= 0 || height <= 0) return;
        const auto region = ::jxx::NEW<::jxx::awt::Rectangle>(
            x, y, width, height);
        for (auto& entry : dirty_)
        {
            if (entry.component != component) continue;
            const auto left = std::min(entry.region->x, x);
            const auto top = std::min(entry.region->y, y);
            const auto right = std::max(
                entry.region->x + entry.region->width, x + width);
            const auto bottom = std::max(
                entry.region->y + entry.region->height, y + height);
            entry.region->setBounds(
                left, top, right - left, bottom - top);
            return;
        }
        dirty_.push_back({component, region});
    }

    ::jxx::Ptr<::jxx::awt::Rectangle> RepaintManager::getDirtyRegion(
        const ::jxx::Ptr<JComponent>& component) const
    {
        for (const auto& entry : dirty_)
            if (entry.component == component)
                return ::jxx::NEW<::jxx::awt::Rectangle>(
                    entry.region->x, entry.region->y,
                    entry.region->width, entry.region->height);
        return ::jxx::NEW<::jxx::awt::Rectangle>();
    }

    void RepaintManager::markCompletelyDirty(
        const ::jxx::Ptr<JComponent>& component)
    {
        if (component != nullptr)
            addDirtyRegion(component, 0, 0,
                component->getWidth(), component->getHeight());
    }

    void RepaintManager::markCompletelyClean(
        const ::jxx::Ptr<JComponent>& component)
    {
        dirty_.erase(
            std::remove_if(dirty_.begin(), dirty_.end(),
                [&component](const DirtyEntry& entry)
                { return entry.component == component; }),
            dirty_.end());
    }

    ::jxx::lang::jbool RepaintManager::isCompletelyDirty(
        const ::jxx::Ptr<JComponent>& component) const
    {
        const auto region = getDirtyRegion(component);
        return component != nullptr
            && region->x <= 0 && region->y <= 0
            && region->width >= component->getWidth()
            && region->height >= component->getHeight();
    }

    void RepaintManager::addInvalidComponent(
        const ::jxx::Ptr<JComponent>& component)
    {
        if (component != nullptr
            && std::find(invalid_.begin(), invalid_.end(), component)
                == invalid_.end())
            invalid_.push_back(component);
    }

    void RepaintManager::removeInvalidComponent(
        const ::jxx::Ptr<JComponent>& component)
    {
        invalid_.erase(
            std::remove(invalid_.begin(), invalid_.end(), component),
            invalid_.end());
    }

    void RepaintManager::validateInvalidComponents()
    {
        const auto components = invalid_;
        invalid_.clear();
        for (const auto& component : components)
            if (component != nullptr) component->validate();
    }

    void RepaintManager::paintDirtyRegions()
    {
        dirty_.clear();
    }

    void RepaintManager::setDoubleBufferingEnabled(
        ::jxx::lang::jbool enabled)
    {
        doubleBufferingEnabled_ = enabled;
    }

    ::jxx::lang::jbool RepaintManager::isDoubleBufferingEnabled() const
    {
        return doubleBufferingEnabled_;
    }

    void RepaintManager::setDoubleBufferMaximumSize(
        const ::jxx::Ptr<::jxx::awt::Dimension>& size)
    {
        doubleBufferMaximumSize_ = size;
    }

    ::jxx::Ptr<::jxx::awt::Dimension>
    RepaintManager::getDoubleBufferMaximumSize() const
    {
        return doubleBufferMaximumSize_ == nullptr
            ? nullptr
            : ::jxx::NEW<::jxx::awt::Dimension>(
                doubleBufferMaximumSize_);
    }
}
