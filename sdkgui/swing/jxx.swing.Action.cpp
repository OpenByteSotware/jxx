#include "swing/jxx.swing.Action.h"

namespace jxx::swing
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> key(const char* value)
        {
            return ::jxx::NEW<::jxx::lang::String>(value);
        }
    }

    ::jxx::Ptr<::jxx::lang::String> Action::NAME = key("Name");
    ::jxx::Ptr<::jxx::lang::String> Action::SMALL_ICON = key("SmallIcon");
    ::jxx::Ptr<::jxx::lang::String> Action::LARGE_ICON_KEY = key("SwingLargeIconKey");
    ::jxx::Ptr<::jxx::lang::String> Action::SHORT_DESCRIPTION = key("ShortDescription");
    ::jxx::Ptr<::jxx::lang::String> Action::LONG_DESCRIPTION = key("LongDescription");
    ::jxx::Ptr<::jxx::lang::String> Action::MNEMONIC_KEY = key("MnemonicKey");
    ::jxx::Ptr<::jxx::lang::String> Action::ACTION_COMMAND_KEY = key("ActionCommandKey");
    ::jxx::Ptr<::jxx::lang::String> Action::ACCELERATOR_KEY = key("AcceleratorKey");
    ::jxx::Ptr<::jxx::lang::String> Action::SELECTED_KEY = key("SwingSelectedKey");
    ::jxx::Ptr<::jxx::lang::String> Action::DISPLAYED_MNEMONIC_INDEX_KEY =
        key("SwingDisplayedMnemonicIndexKey");
}
