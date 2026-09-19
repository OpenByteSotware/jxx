#include "swing/jxx.swing.JFormattedTextField.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    JFormattedTextField::JFormattedTextField() : Super() {}
    JFormattedTextField::JFormattedTextField(
        const ::jxx::Ptr<::jxx::lang::Object>& value) : Super()
    { setValue(value); }
    void JFormattedTextField::setValue(
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        value_ = value;
        setText(value ? value->toString()
            : ::jxx::NEW<::jxx::lang::String>(""));
        editValid_ = true;
    }
    ::jxx::Ptr<::jxx::lang::Object> JFormattedTextField::getValue() const
    { return value_; }
    void JFormattedTextField::setFocusLostBehavior(::jxx::lang::jint value)
    {
        if (value < COMMIT || value > PERSIST)
            throw ::jxx::lang::IllegalArgumentException("behavior");
        focusLostBehavior_ = value;
    }
    ::jxx::lang::jint JFormattedTextField::getFocusLostBehavior() const
    { return focusLostBehavior_; }
    void JFormattedTextField::commitEdit()
    {
        value_ = ::jxx::CAST<::jxx::lang::Object>(getText());
        editValid_ = true;
    }
    ::jxx::lang::jbool JFormattedTextField::isEditValid() const
    { return editValid_; }
}
