#pragma once

#include <vector>

#include "beans/jxx.beans.PropertyChangeListener.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "swing/jxx.swing.Action.h"

namespace jxx::swing
{
    class Icon;

    class AbstractAction : public ::jxx::lang::ClassBase<
        AbstractAction, ::jxx::lang::Object, Action,
        ::jxx::lang::Cloneable, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            AbstractAction, JxxSuper, Action,
            ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;

        AbstractAction();
        explicit AbstractAction(
            const ::jxx::Ptr<::jxx::lang::String>& name);
        AbstractAction(
            const ::jxx::Ptr<::jxx::lang::String>& name,
            const ::jxx::Ptr<Icon>& icon);
        ~AbstractAction() override = default;

        ::jxx::Ptr<::jxx::lang::Object> getValue(
            const ::jxx::Ptr<::jxx::lang::String>& key) const override;
        void putValue(
            const ::jxx::Ptr<::jxx::lang::String>& key,
            const ::jxx::Ptr<::jxx::lang::Object>& value) override;
        void setEnabled(::jxx::lang::jbool enabled) override;
        ::jxx::lang::jbool isEnabled() const override;
        void addPropertyChangeListener(
            const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener)
            override;
        void removePropertyChangeListener(
            const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener)
            override;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    protected:
        void firePropertyChange(
            const ::jxx::Ptr<::jxx::lang::String>& propertyName,
            const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
            const ::jxx::Ptr<::jxx::lang::Object>& newValue);

    private:
        struct Entry
        {
            ::jxx::Ptr<::jxx::lang::String> key;
            ::jxx::Ptr<::jxx::lang::Object> value;
        };
        static ::jxx::lang::jbool sameKey(
            const ::jxx::Ptr<::jxx::lang::String>& left,
            const ::jxx::Ptr<::jxx::lang::String>& right);

        std::vector<Entry> values_;
        std::vector<::jxx::Ptr<::jxx::beans::PropertyChangeListener>>
            listeners_;
        ::jxx::lang::jbool enabled_ = true;
    };
}
