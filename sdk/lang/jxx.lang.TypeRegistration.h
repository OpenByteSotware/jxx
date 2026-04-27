#pragma once

#include "jxx.lang.Class.h"

namespace jxx::lang::detail {

    // Per-type metadata (specialized by macros)
    template <class T> inline constexpr const char* canonical_v = nullptr;
    template <class T> inline constexpr const char* simple_v = nullptr;

    template <class T> inline constexpr bool is_interface_v = false;
    template <class T> inline constexpr bool is_abstract_v = false;
    template <class T> inline constexpr bool abstract_marker_v = false;

    // One TypeInfo per (T) across the entire program
    template <class T>
    inline ::jxx::lang::TypeInfo& typeinfo() {
        static_assert(canonical_v<T> != nullptr,
            "Missing JXX_REGISTER_CLASS/INTERFACE(T, ...) for this T");
        static_assert(simple_v<T> != nullptr,
            "Missing JXX_REGISTER_CLASS/INTERFACE(T, ...) for this T");

        static ::jxx::lang::TypeInfo ti = ::jxx::lang::makeTypeInfo<T>(canonical_v<T>, simple_v<T>);

        // Idempotent flag application
        ti.isInterface = is_interface_v<T>;
        ti.isAbstract = (is_abstract_v<T> || abstract_marker_v<T>);
        return ti;
    }

    // Register exactly once per type per program
    template <class T>
    inline void ensure_registered() {
        static const bool once = [{
            auto& ti = typeinfo<T>();
            ::jxx::lang::TypeRegistry::instance().registerType(&ti);
            return true;
        }]();
        (void)once;
    }

    // Relationships (order independent)
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

    // Default ctor exposure (once)
    template <class T>
    inline void ensure_default_ctor_enabled() {
        static const bool once = [{
            ensure_registered<T>();
            auto& ti = typeinfo<T>();
            ti.defaultCtorAccessible = true;

            // Use your allocator if you have one (JXX_NEW<T>()), otherwise std::make_shared<T>()
            ti.defaultCtor = [->std::shared_ptr<::jxx::lang::Object> {
                return std::static_pointer_cast<::jxx::lang::Object>(std::make_shared<T>());
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

    // Unique symbol helpers
#define JXX_DETAIL_CAT2(a,b) a##b
#define JXX_DETAIL_CAT(a,b)  JXX_DETAIL_CAT2(a,b)

} // namespace jxx::lang::detail


// -----------------------------------------------------------------------------
// Macros (header-safe; work with namespace-qualified T)
// -----------------------------------------------------------------------------

#define JXX_REGISTER_CLASS(T, CANONICAL, SIMPLE)                                  \
namespace jxx::lang::detail {                                                     \
    template <> inline constexpr const char* canonical_v<T> = (CANONICAL);        \
    template <> inline constexpr const char* simple_v<T>    = (SIMPLE);           \
    template <> inline constexpr bool is_interface_v<T>     = false;              \
    template <> inline constexpr bool is_abstract_v<T>      = false;              \
}                                                                                 \
namespace {                                                                       \
    inline const bool                                            \
    JXX_DETAIL_CAT(jxx_reg_, __COUNTER__) =                                       \
        (::jxx::lang::detail::ensure_registered<T>(), true);                      \
}

#define JXX_REGISTER_INTERFACE(T, CANONICAL, SIMPLE)                              \
namespace jxx::lang::detail {                                                     \
    template <> inline constexpr const char* canonical_v<T> = (CANONICAL);        \
    template <> inline constexpr const char* simple_v<T>    = (SIMPLE);           \
    template <> inline constexpr bool is_interface_v<T>     = true;               \
    template <> inline constexpr bool is_abstract_v<T>      = true;               \
}                                                                                 \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_reg_, __COUNTER__) =                                       \
        (::jxx::lang::detail::ensure_registered<T>(), true);                      \
}

#define JXX_MARK_ABSTRACT(T)                                                      \
namespace jxx::lang::detail {                                                     \
    template <> inline constexpr bool abstract_marker_v<T> = true;                \
}                                                                                 \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_abs_, __COUNTER__) =                                       \
        (::jxx::lang::detail::ensure_registered<T>(), true);                      \
}

#define JXX_SET_SUPER(T, SUPER)                                                   \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_super_, __COUNTER__) =                                     \
        (::jxx::lang::detail::ensure_super_set<T, SUPER>(), true);                \
}

#define JXX_ADD_INTERFACE_EDGE(T, IFACE)                                          \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_iface_, __COUNTER__) =                                     \
        (::jxx::lang::detail::ensure_iface_edge<T, IFACE>(), true);               \
}

#define JXX_ENABLE_DEFAULT_CTOR(T)                                                \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_ctor_en_, __COUNTER__) =                                   \
        (::jxx::lang::detail::ensure_default_ctor_enabled<T>(), true);            \
}

#define JXX_DISABLE_DEFAULT_CTOR_ACCESS(T)                                        \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_ctor_dis_, __COUNTER__) =                                  \
        (::jxx::lang::detail::ensure_default_ctor_disabled<T>(), true);           \
}