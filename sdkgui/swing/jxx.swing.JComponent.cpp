#include "swing/jxx.swing.JComponent.h"

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::swing
{
    JComponent::JComponent() = default;
    void JComponent::setToolTipText(const ::jxx::Ptr<::jxx::lang::String>& value) { toolTipText_ = value; }
    ::jxx::Ptr<::jxx::lang::String> JComponent::getToolTipText() const { return toolTipText_; }
    void JComponent::setOpaque(::jxx::lang::jbool value) { opaque_ = value; }
    ::jxx::lang::jbool JComponent::isOpaque() const { return opaque_; }
    void JComponent::setDoubleBuffered(::jxx::lang::jbool value) { doubleBuffered_ = value; }
    ::jxx::lang::jbool JComponent::isDoubleBuffered() const { return doubleBuffered_; }
    void JComponent::putClientProperty(const ::jxx::Ptr<::jxx::lang::Object>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        if (key == nullptr) throw ::jxx::lang::NullPointerException("key");
        if (value == nullptr) clientProperties_.erase(key.get());
        else clientProperties_[key.get()] = value;
    }
    ::jxx::Ptr<::jxx::lang::Object> JComponent::getClientProperty(
        const ::jxx::Ptr<::jxx::lang::Object>& key) const
    {
        if (key == nullptr) return nullptr;
        const auto found = clientProperties_.find(key.get());
        return found == clientProperties_.end() ? nullptr : found->second;
    }
    void JComponent::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
    { out->writeBoolean(opaque_); out->writeBoolean(doubleBuffered_); }
    void JComponent::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)
    { opaque_ = in->readBoolean(); doubleBuffered_ = in->readBoolean(); }
    void JComponent::readObjectNoData() { opaque_ = false; doubleBuffered_ = false; }
}
