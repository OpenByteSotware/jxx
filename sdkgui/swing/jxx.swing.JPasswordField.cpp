#include "swing/jxx.swing.JPasswordField.h"
namespace jxx::swing
{
    JPasswordField::JPasswordField() : Super() {}
    JPasswordField::JPasswordField(const ::jxx::Ptr<::jxx::lang::String>& text)
        : Super(text) {}
    JPasswordField::JPasswordField(::jxx::lang::jint columns)
        : Super(columns) {}
    JPasswordField::JPasswordField(
        const ::jxx::Ptr<::jxx::lang::String>& text,
        ::jxx::lang::jint columns) : Super(text, columns) {}
    void JPasswordField::setEchoChar(::jxx::lang::jchar value) { echoChar_ = value; }
    ::jxx::lang::jchar JPasswordField::getEchoChar() const { return echoChar_; }
    ::jxx::lang::jbool JPasswordField::echoCharIsSet() const { return echoChar_ != 0; }
    JPasswordField::CharArray JPasswordField::getPassword() const
    {
        const auto value = JTextField::getText()->utf8();
        auto result = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jchar, 1>>(
            static_cast<std::uint32_t>(value.size()));
        for (std::uint32_t index = 0; index < result->length; ++index)
            (*result)[index] = static_cast<::jxx::lang::jchar>(
                static_cast<unsigned char>(value[index]));
        return result;
    }
    ::jxx::Ptr<::jxx::lang::String> JPasswordField::getText() const
    {
        return JTextField::getText();
    }
}
