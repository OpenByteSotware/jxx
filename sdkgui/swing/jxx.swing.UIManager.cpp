#include "swing/jxx.swing.UIManager.h"

#include <mutex>

#include "swing/jxx.swing.UIDefaults.h"

namespace jxx::swing
{
    namespace
    {
        std::mutex& defaultsMutex()
        {
            static std::mutex value;
            return value;
        }

        ::jxx::Ptr<UIDefaults>& defaultsInstance()
        {
            static ::jxx::Ptr<UIDefaults> value;
            return value;
        }
    }

    ::jxx::Ptr<UIDefaults> UIManager::getDefaults()
    {
        std::lock_guard<std::mutex> lock(defaultsMutex());
        auto& value = defaultsInstance();
        if (value == nullptr) value = ::jxx::NEW<UIDefaults>();
        return value;
    }

    ::jxx::Ptr<UIDefaults> UIManager::getLookAndFeelDefaults()
    {
        return getDefaults();
    }

    ::jxx::Ptr<::jxx::lang::Object> UIManager::get(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->get(key); }

    ::jxx::Ptr<::jxx::lang::Object> UIManager::put(
        const ::jxx::Ptr<::jxx::lang::Object>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    { return getDefaults()->put(key, value); }

    ::jxx::Ptr<::jxx::lang::String> UIManager::getString(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getString(key); }

    ::jxx::Ptr<::jxx::awt::Color> UIManager::getColor(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getColor(key); }

    ::jxx::Ptr<::jxx::awt::Font> UIManager::getFont(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getFont(key); }

    ::jxx::Ptr<Icon> UIManager::getIcon(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getIcon(key); }

    ::jxx::Ptr<::jxx::awt::Insets> UIManager::getInsets(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getInsets(key); }

    ::jxx::Ptr<::jxx::awt::Dimension> UIManager::getDimension(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getDimension(key); }

    ::jxx::lang::jint UIManager::getInt(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getInt(key); }

    ::jxx::lang::jbool UIManager::getBoolean(
        const ::jxx::Ptr<::jxx::lang::Object>& key)
    { return getDefaults()->getBoolean(key); }
}
