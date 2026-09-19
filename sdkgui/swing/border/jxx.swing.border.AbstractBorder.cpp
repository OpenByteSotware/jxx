#include "swing/border/jxx.swing.border.AbstractBorder.h"

#include "awt/jxx.awt.Insets.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

namespace jxx::swing::border
{
    void AbstractBorder::paintBorder(
        const ::jxx::Ptr<::jxx::awt::Component>&,
        const ::jxx::Ptr<::jxx::awt::Graphics>&,
        ::jxx::lang::jint,
        ::jxx::lang::jint,
        ::jxx::lang::jint,
        ::jxx::lang::jint)
    {
    }

    ::jxx::Ptr<::jxx::awt::Insets> AbstractBorder::getBorderInsets(
        const ::jxx::Ptr<::jxx::awt::Component>&) const
    {
        return ::jxx::NEW<::jxx::awt::Insets>(0, 0, 0, 0);
    }

    ::jxx::Ptr<::jxx::awt::Insets> AbstractBorder::getBorderInsets(
        const ::jxx::Ptr<::jxx::awt::Component>&,
        const ::jxx::Ptr<::jxx::awt::Insets>& insets) const
    {
        const auto result = insets != nullptr
            ? insets
            : ::jxx::NEW<::jxx::awt::Insets>(0, 0, 0, 0);
        result->set(0, 0, 0, 0);
        return result;
    }

    ::jxx::lang::jbool AbstractBorder::isBorderOpaque() const
    {
        return false;
    }

    void AbstractBorder::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void AbstractBorder::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
    }

    void AbstractBorder::readObjectNoData()
    {
    }
}
