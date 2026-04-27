#pragma once

#include <memory>
#include "jxx_types.h"
#include "jxx.lang.ClassCastException.h"

namespace jxx::lang {

    // ---------------------------
    // Java-style instanceof
    // ---------------------------
    //
    // Usage:
    //   if (instanceof_as<Foo>(obj)) { ... }
    //
    // Semantics:
    //   - returns false if obj == null
    //   - returns true if obj is instance of Foo (or implements Foo if Foo is an interface type)
    //
    template <class To, class From>
    inline bool instanceof(const jxx::Ptr<From>& obj) noexcept {
        if (!obj) return false;
        return static_cast<bool>(std::dynamic_pointer_cast<To>(obj));
    }

    // ---------------------------
    // Java-style cast: (To) obj
    // ---------------------------
    //
    // Usage:
    //   auto foo = cast_as<Foo>(obj);     // returns null if obj is null; throws ClassCastException if incompatible
    //
    // Semantics:
    //   - if obj == null: returns null (same as Java)
    //   - else if compatible: returns casted pointer
    //   - else: throws ClassCastException
    //
    template <class To, class From>
    inline jxx::Ptr<To> cast_as(const jxx::Ptr<From>& obj) {
        if (!obj) return jxx::Ptr<To>{}; // Java: (T)null == null

        auto casted = std::dynamic_pointer_cast<To>(obj);
        if (casted) return casted;

        // Java: ClassCastException. Message text is not specified strictly; keep simple and stable.
        // If you want richer messages, you can use a registry or typeName() from Throwable/Object metadata.
        throw jxx::lang::ClassCastException("ClassCastException: incompatible cast")
        );
    }

    // ---------------------------
    // Try-cast (safe cast): returns null instead of throwing
    // ---------------------------
    template <class To, class From>
    inline jxx::Ptr<To> try_cast_as(const jxx::Ptr<From>& obj) noexcept {
        if (!obj) return jxx::Ptr<To>{};
        return std::dynamic_pointer_cast<To>(obj);
    }

} // namespace jxx::lang


#define JXX_INSTANCEOF(TO, OBJ) ::jxx::lang::instanceof<TO>(OBJ)
#define JXX_CAST(TO, OBJ)       ::jxx::lang::cast_as<TO>(OBJ)
#define JXX_TRY_CAST(TO, OBJ)   ::jxx::lang::try_cast_as<TO>(OBJ)

