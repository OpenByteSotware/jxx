#pragma once

#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::swing
{
    class Action;

    class ActionMap final : public ::jxx::lang::ClassBase<
        ActionMap, ::jxx::lang::Object, ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<
            ActionMap, JxxSuper, ::jxx::io::SerializableI>;
        using KeyArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::Ptr<::jxx::lang::Object>, 1>>;

        ActionMap();
        void setParent(const ::jxx::Ptr<ActionMap>& map);
        ::jxx::Ptr<ActionMap> getParent() const;
        void put(
            const ::jxx::Ptr<::jxx::lang::Object>& key,
            const ::jxx::Ptr<Action>& action);
        ::jxx::Ptr<Action> get(
            const ::jxx::Ptr<::jxx::lang::Object>& key) const;
        void remove(const ::jxx::Ptr<::jxx::lang::Object>& key);
        void clear();
        KeyArray keys() const;
        KeyArray allKeys() const;
        ::jxx::lang::jint size() const;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    private:
        struct Entry
        {
            ::jxx::Ptr<::jxx::lang::Object> key;
            ::jxx::Ptr<Action> action;
        };
        static ::jxx::lang::jbool sameKey(
            const ::jxx::Ptr<::jxx::lang::Object>& left,
            const ::jxx::Ptr<::jxx::lang::Object>& right);

        ::jxx::Ptr<ActionMap> parent_;
        std::vector<Entry> values_;
    };
}
