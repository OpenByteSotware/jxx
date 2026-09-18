#pragma once
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    class JOptionPane : public ::jxx::lang::ClassBase<JOptionPane,JComponent>
    {
    public:
        using JxxSuper=JComponent;using Super=::jxx::lang::ClassBase<JOptionPane,JxxSuper>;
        static constexpr ::jxx::lang::jint DEFAULT_OPTION=-1,YES_NO_OPTION=0,YES_NO_CANCEL_OPTION=1,OK_CANCEL_OPTION=2;
        static constexpr ::jxx::lang::jint YES_OPTION=0,NO_OPTION=1,CANCEL_OPTION=2,OK_OPTION=0,CLOSED_OPTION=-1;
        static constexpr ::jxx::lang::jint ERROR_MESSAGE=0,INFORMATION_MESSAGE=1,WARNING_MESSAGE=2,QUESTION_MESSAGE=3,PLAIN_MESSAGE=-1;
        JOptionPane(); explicit JOptionPane(const ::jxx::Ptr<::jxx::lang::Object>& message); JOptionPane(const ::jxx::Ptr<::jxx::lang::Object>& message,::jxx::lang::jint messageType,::jxx::lang::jint optionType);
        void setMessage(const ::jxx::Ptr<::jxx::lang::Object>& message);::jxx::Ptr<::jxx::lang::Object> getMessage()const;
        void setMessageType(::jxx::lang::jint type);::jxx::lang::jint getMessageType()const;void setOptionType(::jxx::lang::jint type);::jxx::lang::jint getOptionType()const;
        void setValue(const ::jxx::Ptr<::jxx::lang::Object>& value);::jxx::Ptr<::jxx::lang::Object> getValue()const;void setInputValue(const ::jxx::Ptr<::jxx::lang::Object>& value);::jxx::Ptr<::jxx::lang::Object> getInputValue()const;void setWantsInput(::jxx::lang::jbool value);::jxx::lang::jbool getWantsInput()const;
        static void showMessageDialog(const ::jxx::Ptr<::jxx::awt::Component>& parent,const ::jxx::Ptr<::jxx::lang::Object>& message);
        static ::jxx::lang::jint showConfirmDialog(const ::jxx::Ptr<::jxx::awt::Component>& parent,const ::jxx::Ptr<::jxx::lang::Object>& message);
        static ::jxx::Ptr<::jxx::lang::Object> showInputDialog(const ::jxx::Ptr<::jxx::lang::Object>& message);
    private:
        ::jxx::Ptr<::jxx::lang::Object> message_,value_,inputValue_;::jxx::lang::jint messageType_=PLAIN_MESSAGE,optionType_=DEFAULT_OPTION;::jxx::lang::jbool wantsInput_=false;
    };
}
