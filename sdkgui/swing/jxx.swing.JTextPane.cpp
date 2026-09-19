#include "swing/jxx.swing.JTextPane.h"
#include <string>
namespace jxx::swing
{
    JTextPane::JTextPane() : Super() {}
    void JTextPane::replaceSelection(
        const ::jxx::Ptr<::jxx::lang::String>& content)
    {
        auto value = getText()->utf8();
        const auto replacement = content ? content->utf8() : std::string();
        const auto position = static_cast<std::size_t>(getCaretPosition());
        value.insert(position > value.size() ? value.size() : position, replacement);
        setText(::jxx::NEW<::jxx::lang::String>(value));
        setCaretPosition(static_cast<::jxx::lang::jint>(
            (position > value.size() ? value.size() : position) + replacement.size()));
    }
    void JTextPane::insertComponent(
        const ::jxx::Ptr<::jxx::awt::Component>& component)
    { lastInsertedComponent_ = component; }
    ::jxx::Ptr<::jxx::awt::Component>
    JTextPane::getLastInsertedComponent() const
    { return lastInsertedComponent_; }
}
