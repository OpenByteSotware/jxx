#pragma once

#include <typeindex>
#include <mutex>
#include <utility>

// Your project headers:
#include "jxx.lang.Class.h"   // TypeInfo, TypeRegistry, Object, String, etc.

// -----------------------------------------------------------------------------
// makeTypeInfo<T>()
// -----------------------------------------------------------------------------
namespace jxx::lang {

    template <typename T>
    inline TypeInfo makeTypeInfo(const char* canonical, const char* simple) {
        TypeInfo ti{
            String(canonical),
            String(simple),
            std::type_index(typeid(T)),
            false,   // isInterface
            false,   // isAbstract
            false,   // isPrimitive
            nullptr, // super
            {},      // interfaces
            false,   // defaultCtorAccessible
            nullptr, // defaultCtor
            // isInstanceFn: cross-cast works if implementing objects inherit both Object and interface T
            [](const std::shared_ptr<Object>& o) -> bool {
                return static_cast<bool>(std::dynamic_pointer_cast<T>(o));
            }
        };
        return ti;
    }

    namespace detail {

        // -------------------------------------------------------------------------
        // Per-type compile-time metadata (header-friendly via inline variable templates)
        // These are specialized by the macros.
        // -------------------------------------------------------------------------
        template <class T> inline constexpr const char* canonical_v = nullptr;
        template <class T> inline constexpr const char* simple_v = nullptr;

        template <class T> inline constexpr bool is_interface_v = false;
        template <class T> inline constexpr bool is_abstract_v = false;

        // Optional “marker” toggles that can be specialized independently (no collisions)
        template <class T> inline constexpr bool abstract_marker_v = false;

        // -------------------------------------------------------------------------
        // Singleton TypeInfo per type (program-wide)
        // -------------------------------------------------------------------------
        template <class T>
        inline TypeInfo& typeinfo() {
            static_assert(canonical_v<T> != nullptr,
                "Type is not registered: missing JXX_REGISTER_CLASS/INTERFACE for this T");
            static_assert(simple_v<T> != nullptr,
                "Type is not registered: missing JXX_REGISTER_CLASS/INTERFACE for this T");

            static TypeInfo ti = ::jxx::lang::makeTypeInfo<T>(canonical_v<T>, simple_v<T>);

            // Apply flags idempotently (safe if multiple TUs call this)
            ti.isInterface = is_interface_v<T>;
            ti.isAbstract = (is_abstract_v<T> || abstract_marker_v<T>);

            return ti;
        }

        // -------------------------------------------------------------------------
        // Register once per type (program-wide)
        // -------------------------------------------------------------------------
        template <class T>
        inline void ensure_registered() {
            static const bool once = [auto& ti = typeinfo<T>();
            ::jxx::lang::TypeRegistry::instance().registerType(&ti);
            return true;
        }();
        (void)once;
    }

    // -------------------------------------------------------------------------
    // Relationships (super / interface edges), order-independent
    // -------------------------------------------------------------------------
    template <class T, class Super>
    inline void ensure_super_set() {
        static const bool once = [{
            ensure_registered<T>();
            ensure_registered<Super>();
            typeinfo<T>().super = &typeinfo<Super>();
            return true;
        }();
        (void)once;
    }

    template <class T, class IFace>
    inline void ensure_iface_edge() {
        static const bool once = [{
            ensure_registered<T>();
            ensure_registered<IFace>();
            typeinfo<T>().interfaces.push_back(&typeinfo<IFace>());
            return true;
        }();
        (void)once;
    }

    // -------------------------------------------------------------------------
    // Default ctor exposure (once-guards)
    // -------------------------------------------------------------------------
    template <class T>
    inline void ensure_default_ctor_enabled() {
        static const bool once = [{
            ensure_registered<T>();
            auto& ti = typeinfo<T>();
            ti.defaultCtorAccessible = true;
            ti.defaultCtor = [->std::shared_ptr<::jxx::lang::Object> {
                return std::static_pointer_cast<::jxx::lang::Object>(JXX_NEW<T>());
            };
            return true;
        }();
        (void)once;
    }

    template <class T>
    inline void ensure_default_ctor_disabled() {
        static const bool once = [{
            ensure_registered<T>();
            typeinfo<T>().defaultCtorAccessible = false;
            return true;
        }();
        (void)once;
    }

    // -------------------------------------------------------------------------
    // Macro helpers for unique symbols
    // -------------------------------------------------------------------------
#define JXX_DETAIL_CAT2(a,b) a##b
#define JXX_DETAIL_CAT(a,b)  JXX_DETAIL_CAT2(a,b)

} // namespace detail
} // namespace jxx::lang