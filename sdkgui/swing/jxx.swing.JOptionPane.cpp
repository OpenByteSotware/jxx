#include "swing/jxx.swing.JOptionPane.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
	JOptionPane::JOptionPane() :Super()
	{
	}JOptionPane::JOptionPane(const ::jxx::Ptr<::jxx::lang::Object>& m) :JOptionPane(m, PLAIN_MESSAGE, DEFAULT_OPTION)
	{
	}JOptionPane::JOptionPane(const ::jxx::Ptr<::jxx::lang::Object>& m, ::jxx::lang::jint mt, ::jxx::lang::jint ot) :Super()
	{
		setMessage(m); setMessageType(mt); setOptionType(ot);
	}void JOptionPane::setMessage(const ::jxx::Ptr<::jxx::lang::Object>& v)
	{
		message_ = v;
	}::jxx::Ptr<::jxx::lang::Object> JOptionPane::getMessage()const
	{
		return message_;
	}void JOptionPane::setMessageType(::jxx::lang::jint v)
	{
		if (v<PLAIN_MESSAGE || v>QUESTION_MESSAGE)throw ::jxx::lang::IllegalArgumentException("messageType"); messageType_ = v;
	}::jxx::lang::jint JOptionPane::getMessageType()const
	{
		return messageType_;
	}void JOptionPane::setOptionType(::jxx::lang::jint v)
	{
		if (v<DEFAULT_OPTION || v>OK_CANCEL_OPTION)throw ::jxx::lang::IllegalArgumentException("optionType"); optionType_ = v;
	}::jxx::lang::jint JOptionPane::getOptionType()const
	{
		return optionType_;
	}void JOptionPane::setValue(const ::jxx::Ptr<::jxx::lang::Object>& v)
	{
		value_ = v;
	}::jxx::Ptr<::jxx::lang::Object> JOptionPane::getValue()const
	{
		return value_;
	}
	void JOptionPane::setInputValue(const ::jxx::Ptr<::jxx::lang::Object>& v)
	{
		inputValue_ = v;
	}::jxx::Ptr<::jxx::lang::Object> JOptionPane::getInputValue()const
	{
		return inputValue_;
	}void JOptionPane::setWantsInput(::jxx::lang::jbool v)
	{
		wantsInput_ = v;
	}::jxx::lang::jbool JOptionPane::getWantsInput()const
	{
		return wantsInput_;
	}
	void JOptionPane::showMessageDialog(const ::jxx::Ptr<::jxx::awt::Component>&, const ::jxx::Ptr<::jxx::lang::Object>&)
	{
	}::jxx::lang::jint JOptionPane::showConfirmDialog(const ::jxx::Ptr<::jxx::awt::Component>&, const ::jxx::Ptr<::jxx::lang::Object>&)
	{
		return CLOSED_OPTION;
	}::jxx::Ptr<::jxx::lang::Object> JOptionPane::showInputDialog(const ::jxx::Ptr<::jxx::lang::Object>&)
	{
		return nullptr;
	}
}
