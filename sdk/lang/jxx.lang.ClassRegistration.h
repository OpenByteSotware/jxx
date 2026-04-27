#pragma once
#include <string>
#include <unordered_map>
#include "jxx.lang.TypeRegistration.h"
#include "jxx.lang.Class.h"

namespace jxx::lang {
    class ClassInfo {
    public:
        std::string cpp_name;
        std::string java_name;
        std::string simple_name;
        const std::type_info* type_info;
    };

    // Registry to store class information
    inline std::unordered_map<std::string, ClassInfo>& GetClassRegistry() {
        static std::unordered_map<std::string, ClassInfo> registry;
        return registry;
    }

    // Helper function to register a class
    inline bool RegisterClass(const std::string& cppName, const std::string& javaName, const std::string& simpleName) {
        GetClassRegistry()[cppName] = { javaName, simpleName };
        return true;
    }

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
        {},      // interfaces edges
        false,   // defaultCtorAccessible
        nullptr, // defaultCtor
        // isInstanceFn: cross-cast works if implementing objects inherit both Object and interface T
        const std::shared_ptr<Object>& o -> bool {
            return (bool)std::dynamic_pointer_cast<T>(o);
        }
    };
    return ti;
}

} // namespace jxx::lang::detail

// Register concrete class type
#define JXX_REGISTER_CLASS(T, CANONICAL, SIMPLE)                                  \
namespace jxx::lang::detail {                                                     \
    template <> inline constexpr const char* canonical_v<T> = (CANONICAL);        \
    template <> inline constexpr const char* simple_v<T>    = (SIMPLE);           \
    template <> inline constexpr bool is_interface_v<T>     = false;              \
    template <> inline constexpr bool is_abstract_v<T>      = false;              \
}                                                                                 \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_reg_, __COUNTER__) =                                       \
        (::jxx::lang::detail::ensure_registered<T>(), true);                      \
}

// Register interface type (does NOT derive Object)
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

// Mark abstract class (optional, can be used after JXX_REGISTER_CLASS)
#define JXX_MARK_ABSTRACT(T)                                                      \
namespace jxx::lang::detail {                                                     \
    template <> inline constexpr bool abstract_marker_v<T> = true;                \
}                                                                                 \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_abs_, __COUNTER__) =                                       \
        (::jxx::lang::detail::ensure_registered<T>(), true);                      \
}

// Set superclass for classes (type-based)
#define JXX_SET_SUPER(T, SUPER)                                                   \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_super_, __COUNTER__) =                                     \
        (::jxx::lang::detail::ensure_super_set<T, SUPER>(), true);                \
}

// Interface edge:
//  - class implements interface
//  - interface extends interface
#define JXX_ADD_INTERFACE_EDGE(T, IFACE)                                          \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_iface_, __COUNTER__) =                                     \
        (::jxx::lang::detail::ensure_iface_edge<T, IFACE>(), true);               \
}

// Expose default ctor via JXX_NEW<T>()
#define JXX_ENABLE_DEFAULT_CTOR(T)                                                \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_ctor_en_, __COUNTER__) =                                   \
        (::jxx::lang::detail::ensure_default_ctor_enabled<T>(), true);            \
}

// If ctor exists but is not accessible (private) — still registered but blocked
#define JXX_DISABLE_DEFAULT_CTOR_ACCESS(T)                                        \
namespace {                                                                       \
    [[maybe_unused]] inline const bool                                            \
    JXX_DETAIL_CAT(jxx_ctor_dis_, __COUNTER__) =                                  \
        (::jxx::lang::detail::ensure_default_ctor_disabled<T>(), true);           \
}