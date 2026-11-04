#include "awt/event/jxx.awt.event.HierarchyEvent.h"

#include <sstream>

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt::event
{
    HierarchyEvent::HierarchyEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::awt::Component>& changed,
        const ::jxx::Ptr<::jxx::awt::Container>& changedParent)
        : HierarchyEvent(source, id, changed, changedParent, 0)
    {
    }

    HierarchyEvent::HierarchyEvent(
        const ::jxx::Ptr<::jxx::awt::Component>& source,
        ::jxx::lang::jint id,
        const ::jxx::Ptr<::jxx::awt::Component>& changed,
        const ::jxx::Ptr<::jxx::awt::Container>& changedParent,
        ::jxx::lang::jlong changeFlags)
        : Super(source, id),
          changed_(changed),
          changedParent_(changedParent),
          changeFlags_(changeFlags)
    {
    }

    ::jxx::Ptr<::jxx::awt::Component> HierarchyEvent::getComponent() const
    {
        return ::jxx::CAST<::jxx::awt::Component>(getSource());
    }

    ::jxx::Ptr<::jxx::awt::Component> HierarchyEvent::getChanged() const
    {
        return changed_;
    }

    ::jxx::Ptr<::jxx::awt::Container> HierarchyEvent::getChangedParent() const
    {
        return changedParent_;
    }

    ::jxx::lang::jlong HierarchyEvent::getChangeFlags() const
    {
        return changeFlags_;
    }

    ::jxx::Ptr<::jxx::lang::String> HierarchyEvent::paramString() const
    {
        const char* name = id == HIERARCHY_CHANGED ? "HIERARCHY_CHANGED"
            : id == ANCESTOR_MOVED ? "ANCESTOR_MOVED"
            : id == ANCESTOR_RESIZED ? "ANCESTOR_RESIZED"
            : "unknown type";
        std::ostringstream out;
        out << name << ",changeFlags=" << changeFlags_;
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
    void HierarchyEvent::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output){if(output==nullptr)throw ::jxx::lang::NullPointerException();JxxSuper::writeObject(output);output->writeObject(::jxx::CAST<::jxx::lang::Object>(changed_));output->writeObject(::jxx::CAST<::jxx::lang::Object>(changedParent_));output->writeLong(changeFlags_);}
    void HierarchyEvent::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input){if(input==nullptr)throw ::jxx::lang::NullPointerException();JxxSuper::readObject(input);changed_=::jxx::CAST<::jxx::awt::Component>(input->readObject());changedParent_=::jxx::CAST<::jxx::awt::Container>(input->readObject());changeFlags_=input->readLong();}
    void HierarchyEvent::readObjectNoData(){JxxSuper::readObjectNoData();changed_=nullptr;changedParent_=nullptr;changeFlags_=0;}
}
