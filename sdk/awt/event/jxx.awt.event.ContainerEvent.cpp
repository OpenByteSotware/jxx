#include "awt/event/jxx.awt.event.ContainerEvent.h"

#include <sstream>

#include "awt/jxx.awt.Container.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt::event
{
    ContainerEvent::ContainerEvent(
        const ::jxx::Ptr<::jxx::awt::Container>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::awt::Component>& child)
        : Super(source, id),
          child_(child)
    {
    }

    ::jxx::Ptr<::jxx::awt::Container> ContainerEvent::getContainer() const
    {
        return ::jxx::CAST<::jxx::awt::Container>(getSource());
    }

    ::jxx::Ptr<::jxx::awt::Component> ContainerEvent::getChild() const
    {
        return child_;
    }

    ::jxx::Ptr<::jxx::lang::String> ContainerEvent::paramString() const
    {
        const char* name = "unknown type";
        if (id == COMPONENT_ADDED) name = "COMPONENT_ADDED";
        else if (id == COMPONENT_REMOVED) name = "COMPONENT_REMOVED";

        std::ostringstream out;
        out << name << ",child=";
        if (child_ != nullptr) out << child_->toString()->utf8();
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
    void ContainerEvent::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output){if(output==nullptr)throw ::jxx::lang::NullPointerException();JxxSuper::writeObject(output);output->writeObject(::jxx::CAST<::jxx::lang::Object>(child_));}
    void ContainerEvent::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input){if(input==nullptr)throw ::jxx::lang::NullPointerException();JxxSuper::readObject(input);child_=::jxx::CAST<::jxx::awt::Component>(input->readObject());}
    void ContainerEvent::readObjectNoData(){JxxSuper::readObjectNoData();child_=nullptr;}
}
