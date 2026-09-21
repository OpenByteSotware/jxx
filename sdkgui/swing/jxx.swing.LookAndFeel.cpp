#include "swing/jxx.swing.LookAndFeel.h"

#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.LayoutStyle.h"
#include "swing/jxx.swing.UIDefaults.h"
#include "swing/jxx.swing.UIManager.h"

namespace jxx::swing
{
    LookAndFeel::LookAndFeel() : Super()
    {
    }

    ::jxx::Ptr<UIDefaults> LookAndFeel::getDefaults()
    {
        if (defaults_ == nullptr) defaults_ = ::jxx::NEW<UIDefaults>();
        return defaults_;
    }

    ::jxx::Ptr<LayoutStyle> LookAndFeel::getLayoutStyle()
    {
        return LayoutStyle::getInstance();
    }

    void LookAndFeel::initialize()
    {
    }

    void LookAndFeel::uninitialize()
    {
    }

    void LookAndFeel::installColors(
        const ::jxx::Ptr<JComponent>& component,
        const ::jxx::Ptr<::jxx::lang::String>& backgroundKey,
        const ::jxx::Ptr<::jxx::lang::String>& foregroundKey)
    {
        if (component == nullptr) return;
        const auto background = UIManager::getColor(backgroundKey);
        const auto foreground = UIManager::getColor(foregroundKey);
        if (background != nullptr) component->setBackground(background);
        if (foreground != nullptr) component->setForeground(foreground);
    }

    void LookAndFeel::installColorsAndFont(
        const ::jxx::Ptr<JComponent>& component,
        const ::jxx::Ptr<::jxx::lang::String>& backgroundKey,
        const ::jxx::Ptr<::jxx::lang::String>& foregroundKey,
        const ::jxx::Ptr<::jxx::lang::String>& fontKey)
    {
        installColors(component, backgroundKey, foregroundKey);
        if (component == nullptr) return;
        const auto font = UIManager::getFont(fontKey);
        if (font != nullptr) component->setFont(font);
    }

    void LookAndFeel::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)
    {
    }

    void LookAndFeel::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)
    {
        defaults_.reset();
    }

    void LookAndFeel::readObjectNoData()
    {
        defaults_.reset();
    }
}
