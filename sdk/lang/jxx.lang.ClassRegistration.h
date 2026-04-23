#pragma once
#include <string>
#include <unordered_map>
#include "jxx.lang.Class.h"

namespace jxx::lang {
    struct ClassInfo {
        std::string javaName;
        std::string simpleName;
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
#define JXX_REGISTER_CLASS(T, CANONICAL, SIMPLE)                                \
    namespace {                                                                 \
        ::jxx::lang::TypeInfo T##_typeinfo = ::jxx::lang::makeTypeInfo<T>(CANONICAL, SIMPLE); \
        struct T##_registrar {                                                  \
            T##_registrar() {                                                   \
                T##_typeinfo.isInterface = false;                               \
                ::jxx::lang::TypeRegistry::instance().registerType(&T##_typeinfo); \
            }                                                                   \
        };                                                                      \
        static T##_registrar T##_registrar_instance;                            \
    }

// Register interface type (does NOT derive Object)
#define JXX_REGISTER_INTERFACE(T, CANONICAL, SIMPLE)                            \
    namespace {                                                                 \
        ::jxx::lang::TypeInfo T##_typeinfo = ::jxx::lang::makeTypeInfo<T>(CANONICAL, SIMPLE); \
        struct T##_registrar {                                                  \
            T##_registrar() {                                                   \
                T##_typeinfo.isInterface = true;                                \
                T##_typeinfo.isAbstract = true;                                 \
                ::jxx::lang::TypeRegistry::instance().registerType(&T##_typeinfo); \
            }                                                                   \
        };                                                                      \
        static T##_registrar T##_registrar_instance;                            \
    }

// Mark abstract class (optional)
#define JXX_MARK_ABSTRACT(T)                                                    \
    namespace {                                                                 \
        struct T##_abstract_marker {                                            \
            T##_abstract_marker() { T##_typeinfo.isAbstract = true; }           \
        };                                                                      \
        static T##_abstract_marker T##_abstract_marker_instance;                \
    }

// Set superclass for classes (canonical Java: every class ultimately extends java.lang.Object)
#define JXX_SET_SUPER(T, SUPER)                                                 \
    namespace {                                                                 \
        struct T##_super_setter {                                               \
            T##_super_setter() { T##_typeinfo.super = &SUPER##_typeinfo; }      \
        };                                                                      \
        static T##_super_setter T##_super_setter_instance;                      \
    }

// Interface edge:\n//  - class implements interface\n//  - interface extends interface\n#define JXX_ADD_INTERFACE_EDGE(T, IFACE)                                       \
    namespace {                                                                 \
        struct T##_iface_adder_##IFACE {                                        \
            T##_iface_adder_##IFACE() { T##_typeinfo.interfaces.push_back(&IFACE##_typeinfo); } \
        };                                                                      \
        static T##_iface_adder_##IFACE T##_iface_adder_##IFACE##_instance;      \
    }

// Expose default ctor via JXX_NEW<T>()
#define JXX_ENABLE_DEFAULT_CTOR(T)                                              \
    namespace {                                                                 \
        struct T##_ctor_enabler {                                               \
            T##_ctor_enabler() {                                                \
                T##_typeinfo.defaultCtorAccessible = true;                      \
                T##_typeinfo.defaultCtor = [-> std::shared_ptr<::jxx::lang::Object> { \
                    return std::static_pointer_cast<::jxx::lang::Object>(JXX_NEW<T>()); \
                };                                                              \
            }                                                                   \
        };                                                                      \
        static T##_ctor_enabler T##_ctor_enabler_instance;                      \
    }

// If ctor exists but is not accessible (private) — still registered but blocked
#define JXX_DISABLE_DEFAULT_CTOR_ACCESS(T)                                      \
    namespace {                                                                 \
        struct T##_ctor_disabler {                                              \
            T##_ctor_disabler() { T##_typeinfo.defaultCtorAccessible = false; } \
        };                                                                      \
        static T##_ctor_disabler T##_ctor_disabler_instance;                    \
    }