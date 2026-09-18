#pragma once
#include <vector>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::swing
{
    class JToggleButton;
    class ButtonGroup : public ::jxx::lang::ClassBase<ButtonGroup, ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<ButtonGroup, JxxSuper, ::jxx::io::SerializableI>;
        ButtonGroup() = default;
        void add(const ::jxx::Ptr<JToggleButton>& button);
        void remove(const ::jxx::Ptr<JToggleButton>& button);
        void clearSelection();
        ::jxx::lang::jint getButtonCount() const;
        ::jxx::Ptr<JToggleButton> getSelection() const;
        ::jxx::lang::jbool isSelected(const ::jxx::Ptr<JToggleButton>& button) const;
        void setSelected(const ::jxx::Ptr<JToggleButton>& button, ::jxx::lang::jbool selected);
        void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;
    private:
        std::vector<::jxx::Ptr<JToggleButton>> buttons_;
        ::jxx::Ptr<JToggleButton> selection_;
    };
}
