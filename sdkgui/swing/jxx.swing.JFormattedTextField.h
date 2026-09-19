#pragma once
#include "swing/jxx.swing.JTextField.h"
namespace jxx::swing
{
    class JFormattedTextField : public ::jxx::lang::ClassBase<
        JFormattedTextField, JTextField>
    {
    public:
        using JxxSuper = JTextField;
        using Super = ::jxx::lang::ClassBase<JFormattedTextField, JxxSuper>;
        static constexpr ::jxx::lang::jint COMMIT = 0;
        static constexpr ::jxx::lang::jint COMMIT_OR_REVERT = 1;
        static constexpr ::jxx::lang::jint REVERT = 2;
        static constexpr ::jxx::lang::jint PERSIST = 3;
        JFormattedTextField();
        explicit JFormattedTextField(
            const ::jxx::Ptr<::jxx::lang::Object>& value);
        void setValue(const ::jxx::Ptr<::jxx::lang::Object>& value);
        ::jxx::Ptr<::jxx::lang::Object> getValue() const;
        void setFocusLostBehavior(::jxx::lang::jint behavior);
        ::jxx::lang::jint getFocusLostBehavior() const;
        void commitEdit();
        ::jxx::lang::jbool isEditValid() const;
    private:
        ::jxx::Ptr<::jxx::lang::Object> value_;
        ::jxx::lang::jint focusLostBehavior_ = COMMIT_OR_REVERT;
        ::jxx::lang::jbool editValid_ = true;
    };
}
