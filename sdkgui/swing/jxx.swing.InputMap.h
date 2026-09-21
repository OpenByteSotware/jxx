#pragma once

#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::swing
{
    class KeyStroke;

    class InputMap : public ::jxx::lang::ClassBase<
        InputMap, ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            InputMap, JxxSuper, ::jxx::io::SerializableI>;
        using KeyStrokeArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<KeyStroke>, 1>>;

        InputMap();
        ~InputMap() override = default;

        virtual void setParent(const ::jxx::Ptr<InputMap>& map);
        ::jxx::Ptr<InputMap> getParent() const;

        virtual void put(
            const ::jxx::Ptr<KeyStroke>& keyStroke,
            const ::jxx::Ptr<::jxx::lang::Object>& actionMapKey);
        ::jxx::Ptr<::jxx::lang::Object> get(
            const ::jxx::Ptr<KeyStroke>& keyStroke) const;
        virtual void remove(const ::jxx::Ptr<KeyStroke>& keyStroke);
        virtual void clear();
        KeyStrokeArray keys() const;
        KeyStrokeArray allKeys() const;
        ::jxx::lang::jint size() const;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    protected:
        struct Entry
        {
            ::jxx::Ptr<KeyStroke> key;
            ::jxx::Ptr<::jxx::lang::Object> value;
        };

        static ::jxx::lang::jbool sameKey(
            const ::jxx::Ptr<KeyStroke>& left,
            const ::jxx::Ptr<KeyStroke>& right);
        const std::vector<Entry>& entries() const noexcept;

    private:
        std::vector<Entry> entries_;
        ::jxx::Ptr<InputMap> parent_;
    };
}
