#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Object.h"
#include "swing/border/jxx.swing.border.Border.h"

namespace jxx::swing::border
{
    class AbstractBorder : public ::jxx::lang::ClassBase<
        AbstractBorder,
        ::jxx::lang::Object,
        Border,
        ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            AbstractBorder,
            JxxSuper,
            Border,
            ::jxx::io::SerializableI>;

        ~AbstractBorder() override = default;

        void paintBorder(
            const ::jxx::Ptr<::jxx::awt::Component>& component,
            const ::jxx::Ptr<::jxx::awt::Graphics>& graphics,
            ::jxx::lang::jint x,
            ::jxx::lang::jint y,
            ::jxx::lang::jint width,
            ::jxx::lang::jint height) override;

        ::jxx::Ptr<::jxx::awt::Insets> getBorderInsets(
            const ::jxx::Ptr<::jxx::awt::Component>& component) const override;

        virtual ::jxx::Ptr<::jxx::awt::Insets> getBorderInsets(
            const ::jxx::Ptr<::jxx::awt::Component>& component,
            const ::jxx::Ptr<::jxx::awt::Insets>& insets) const;

        ::jxx::lang::jbool isBorderOpaque() const override;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;

        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;

        void readObjectNoData() override;
    };
}
