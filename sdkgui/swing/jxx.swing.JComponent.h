#pragma once

#include <unordered_map>

#include "awt/jxx.awt.Container.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::swing
{
    class ActionMap;
    class ComponentInputMap;
    class InputMap;

    class JComponent : public ::jxx::lang::ClassBase<JComponent,
        ::jxx::awt::Container, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::awt::Container;
        using Super = ::jxx::lang::ClassBase<JComponent, JxxSuper,
            ::jxx::io::SerializableI>;

        static constexpr ::jxx::lang::jint UNDEFINED_CONDITION = -1;
        static constexpr ::jxx::lang::jint WHEN_FOCUSED = 0;
        static constexpr ::jxx::lang::jint WHEN_ANCESTOR_OF_FOCUSED_COMPONENT = 1;
        static constexpr ::jxx::lang::jint WHEN_IN_FOCUSED_WINDOW = 2;

        JComponent();
        ~JComponent() override = default;
        void setToolTipText(const ::jxx::Ptr<::jxx::lang::String>& text);
        ::jxx::Ptr<::jxx::lang::String> getToolTipText() const;
        void setOpaque(::jxx::lang::jbool opaque);
        ::jxx::lang::jbool isOpaque() const;
        void setDoubleBuffered(::jxx::lang::jbool value);
        ::jxx::lang::jbool isDoubleBuffered() const;
        void putClientProperty(const ::jxx::Ptr<::jxx::lang::Object>& key,
            const ::jxx::Ptr<::jxx::lang::Object>& value);
        ::jxx::Ptr<::jxx::lang::Object> getClientProperty(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        ::jxx::Ptr<InputMap> getInputMap() const;
        ::jxx::Ptr<InputMap> getInputMap(::jxx::lang::jint condition) const;
        void setInputMap(::jxx::lang::jint condition,
            const ::jxx::Ptr<InputMap>& map);
        ::jxx::Ptr<ActionMap> getActionMap() const;
        void setActionMap(const ::jxx::Ptr<ActionMap>& map);

        void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        friend class ComponentInputMap;
        void revalidateInputMap(const ::jxx::Ptr<ComponentInputMap>& map);
        ::jxx::Ptr<::jxx::lang::String> toolTipText_;
        ::jxx::lang::jbool opaque_ = false;
        ::jxx::lang::jbool doubleBuffered_ = false;
        ::jxx::Ptr<InputMap> focusedInputMap_;
        ::jxx::Ptr<InputMap> ancestorInputMap_;
        ::jxx::Ptr<ComponentInputMap> windowInputMap_;
        ::jxx::Ptr<ActionMap> actionMap_;
        std::unordered_map<const ::jxx::lang::Object*,
            ::jxx::Ptr<::jxx::lang::Object>> clientProperties_;
    };
}
