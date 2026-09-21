#pragma once

#include <vector>

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Rectangle.h"

namespace jxx::awt
{
    class Component;
}

namespace jxx::swing
{
    class JComponent;

    class RepaintManager : public ::jxx::lang::ClassBase<
        RepaintManager, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<RepaintManager, JxxSuper>;

        RepaintManager();
        static ::jxx::Ptr<RepaintManager> currentManager(
            const ::jxx::Ptr<::jxx::awt::Component>& component);
        static ::jxx::Ptr<RepaintManager> currentManager(
            const ::jxx::Ptr<JComponent>& component);
        static void setCurrentManager(
            const ::jxx::Ptr<RepaintManager>& manager);

        void addDirtyRegion(
            const ::jxx::Ptr<JComponent>& component,
            ::jxx::lang::jint x,
            ::jxx::lang::jint y,
            ::jxx::lang::jint width,
            ::jxx::lang::jint height);
        ::jxx::Ptr<::jxx::awt::Rectangle> getDirtyRegion(
            const ::jxx::Ptr<JComponent>& component) const;
        void markCompletelyDirty(
            const ::jxx::Ptr<JComponent>& component);
        void markCompletelyClean(
            const ::jxx::Ptr<JComponent>& component);
        ::jxx::lang::jbool isCompletelyDirty(
            const ::jxx::Ptr<JComponent>& component) const;
        void addInvalidComponent(
            const ::jxx::Ptr<JComponent>& component);
        void removeInvalidComponent(
            const ::jxx::Ptr<JComponent>& component);
        void validateInvalidComponents();
        void paintDirtyRegions();
        void setDoubleBufferingEnabled(::jxx::lang::jbool enabled);
        ::jxx::lang::jbool isDoubleBufferingEnabled() const;
        void setDoubleBufferMaximumSize(
            const ::jxx::Ptr<::jxx::awt::Dimension>& size);
        ::jxx::Ptr<::jxx::awt::Dimension>
            getDoubleBufferMaximumSize() const;

    private:
        struct DirtyEntry
        {
            ::jxx::Ptr<JComponent> component;
            ::jxx::Ptr<::jxx::awt::Rectangle> region;
        };
        std::vector<DirtyEntry> dirty_;
        std::vector<::jxx::Ptr<JComponent>> invalid_;
        ::jxx::lang::jbool doubleBufferingEnabled_ = true;
        ::jxx::Ptr<::jxx::awt::Dimension> doubleBufferMaximumSize_;
    };
}
