#pragma once

#include <cstdint>
#include <unordered_map>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::awt
{
    class RenderingHints final :
        public ::jxx::lang::ClassBase<RenderingHints,
            ::jxx::lang::Object, ::jxx::lang::Cloneable>
    {
    public:
        class Key : public ::jxx::lang::ClassBase<Key, ::jxx::lang::Object>
        {
        public:
            using JxxSuper = ::jxx::lang::Object;
            using Super = ::jxx::lang::ClassBase<Key, JxxSuper>;
            ~Key() override = default;

            virtual ::jxx::lang::jbool isCompatibleValue(
                const ::jxx::Ptr<::jxx::lang::Object>& value) const = 0;
            ::jxx::lang::jbool equals(
                const ::jxx::Ptr<::jxx::lang::Object>& object) const final;
            ::jxx::lang::jint hashCode() const final;

        protected:
            explicit Key(::jxx::lang::jint privateKey);
            ::jxx::lang::jint intKey() const;

        private:
            ::jxx::lang::jint privateKey_;
        };

        RenderingHints();
        RenderingHints(const ::jxx::Ptr<Key>& key,
            const ::jxx::Ptr<::jxx::lang::Object>& value);
        RenderingHints(const RenderingHints& other);

        ::jxx::lang::jint size() const;
        ::jxx::lang::jbool isEmpty() const;
        ::jxx::lang::jbool containsKey(const ::jxx::Ptr<Key>& key) const;
        ::jxx::Ptr<::jxx::lang::Object> get(const ::jxx::Ptr<Key>& key) const;
        ::jxx::Ptr<::jxx::lang::Object> put(const ::jxx::Ptr<Key>& key,
            const ::jxx::Ptr<::jxx::lang::Object>& value);
        ::jxx::Ptr<::jxx::lang::Object> remove(const ::jxx::Ptr<Key>& key);
        void clear();
        void add(const ::jxx::Ptr<RenderingHints>& hints);
        ::jxx::Ptr<::jxx::lang::Object> clone() const override;
        ::jxx::lang::jbool equals(
            const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
        ::jxx::lang::jint hashCode() const override;

    private:
        struct KeyHash
        {
            std::size_t operator()(const ::jxx::Ptr<Key>& key) const noexcept;
        };
        struct KeyEqual
        {
            bool operator()(const ::jxx::Ptr<Key>& left,
                const ::jxx::Ptr<Key>& right) const noexcept;
        };
        std::unordered_map<::jxx::Ptr<Key>,
            ::jxx::Ptr<::jxx::lang::Object>, KeyHash, KeyEqual> values_;
    };
}
