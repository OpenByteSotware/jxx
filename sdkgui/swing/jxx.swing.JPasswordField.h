#pragma once
#include "lang/jxx.lang.buildin_array.h"
#include "swing/jxx.swing.JTextField.h"
namespace jxx::swing
{
    class JPasswordField : public ::jxx::lang::ClassBase<JPasswordField, JTextField>
    {
    public:
        using JxxSuper = JTextField;
        using Super = ::jxx::lang::ClassBase<JPasswordField, JxxSuper>;
        using CharArray = ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jchar, 1>>;
        JPasswordField();
        explicit JPasswordField(const ::jxx::Ptr<::jxx::lang::String>& text);
        explicit JPasswordField(::jxx::lang::jint columns);
        JPasswordField(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint columns);
        void setEchoChar(::jxx::lang::jchar character);
        ::jxx::lang::jchar getEchoChar() const;
        ::jxx::lang::jbool echoCharIsSet() const;
        CharArray getPassword() const;
        ::jxx::Ptr<::jxx::lang::String> getText() const;
    private:
        ::jxx::lang::jchar echoChar_ = static_cast<::jxx::lang::jchar>('*');
    };
}
