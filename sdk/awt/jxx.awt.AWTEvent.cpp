#include "awt/jxx.awt.AWTEvent.h"
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::awt
{
	AWTEvent::AWTEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint value) :Super(source), id(value), consumed(false)
	{
	}
	::jxx::lang::jint AWTEvent::getID() const
	{
		return id;
	} ::jxx::lang::jbool AWTEvent::isConsumed() const
	{
		return consumed;
	} void AWTEvent::consume()
	{
		consumed = true;
	}
	::jxx::Ptr<::jxx::lang::String> AWTEvent::paramString() const
	{
		return ::jxx::NEW<::jxx::lang::String>("");
	}
	::jxx::Ptr<::jxx::lang::String> AWTEvent::toString() const
	{
		std::ostringstream out; out << "jxx.awt.AWTEvent[" << paramString()->utf8() << "] on " << getSource()->toString()->utf8(); return ::jxx::NEW<::jxx::lang::String>(out.str());
	}
    void AWTEvent::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output)
    {
        if (output == nullptr) throw ::jxx::lang::NullPointerException();
        JxxSuper::writeObject(output);
        output->writeInt(id);
        output->writeBoolean(consumed);
    }

    void AWTEvent::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input)
    {
        if (input == nullptr) throw ::jxx::lang::NullPointerException();
        JxxSuper::readObject(input);
        id = input->readInt();
        consumed = input->readBoolean();
    }

    void AWTEvent::readObjectNoData()
    {
        JxxSuper::readObjectNoData();
        id = 0;
        consumed = false;
    }
}

