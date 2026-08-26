#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {

template <
    typename Derived,
    typename JavaSuper,
    typename... JavaInterfaces>
class ClassInfo;

namespace class_info_detail {

template <typename T>
constexpr std::string_view rawTypeName() noexcept {
#if defined(__clang__)
    constexpr std::string_view signature = __PRETTY_FUNCTION__;
    constexpr std::string_view marker = "T = ";
    const std::size_t begin = signature.find(marker) + marker.size();
    const std::size_t end = signature.find(']', begin);
    return signature.substr(begin, end - begin);
#elif defined(__GNUC__)
    constexpr std::string_view signature = __PRETTY_FUNCTION__;
    constexpr std::string_view marker = "T = ";
    const std::size_t begin = signature.find(marker) + marker.size();
    std::size_t end = signature.find(';', begin);
    if (end == std::string_view::npos) {
        end = signature.find(']', begin);
    }
    return signature.substr(begin, end - begin);
#elif defined(_MSC_VER)
    constexpr std::string_view signature = __FUNCSIG__;
    constexpr std::string_view marker = "rawTypeName<";
    const std::size_t begin = signature.find(marker) + marker.size();
    const std::size_t end = signature.find(">(void)", begin);
    return signature.substr(begin, end - begin);
#else
#   error ClassInfo requires MSVC, GCC, or Clang.
#endif
}

constexpr bool startsWith(
    std::string_view value,
    std::string_view prefix) noexcept {

    return value.size() >= prefix.size() &&
        value.substr(0, prefix.size()) == prefix;
}

constexpr std::string_view removeLeadingTypeKeyword(
    std::string_view value) noexcept {

    constexpr std::string_view classPrefix = "class ";
    constexpr std::string_view structPrefix = "struct ";
    constexpr std::string_view enumPrefix = "enum ";

    if (startsWith(value, classPrefix)) {
        return value.substr(classPrefix.size());
    }
    if (startsWith(value, structPrefix)) {
        return value.substr(structPrefix.size());
    }
    if (startsWith(value, enumPrefix)) {
        return value.substr(enumPrefix.size());
    }
    return value;
}

template <typename T>
constexpr std::string_view completeTypeNameView() noexcept {
    return removeLeadingTypeKeyword(rawTypeName<T>());
}

constexpr std::size_t findLastOuterNamespaceSeparator(
    std::string_view value) noexcept {

    std::size_t result = std::string_view::npos;
    std::size_t templateDepth = 0;

    for (std::size_t index = 0;
         index + 1 < value.size();
         ++index) {

        if (value[index] == '<') {
            ++templateDepth;
            continue;
        }
        if (value[index] == '>') {
            if (templateDepth > 0) {
                --templateDepth;
            }
            continue;
        }
        if (templateDepth == 0 &&
            value[index] == ':' &&
            value[index + 1] == ':') {

            result = index;
            ++index;
        }
    }

    return result;
}

template <typename T>
constexpr std::string_view simpleTypeNameView() noexcept {
    constexpr auto full = completeTypeNameView<T>();
    constexpr auto separator = findLastOuterNamespaceSeparator(full);

    if constexpr (separator == std::string_view::npos) {
        return full;
    } else {
        return full.substr(separator + 2);
    }
}

template <typename T>
const std::string& binaryTypeName() {
    static const std::string name = [] {
        constexpr auto raw = completeTypeNameView<T>();
        std::string result;
        result.reserve(raw.size());

        for (std::size_t index = 0;
             index < raw.size();
             ++index) {

            if (index + 1 < raw.size() &&
                raw[index] == ':' &&
                raw[index + 1] == ':') {

                result.push_back('.');
                ++index;
            } else if (raw[index] != ' ') {
                result.push_back(raw[index]);
            }
        }

        return result;
    }();

    return name;
}

template <typename T>
const std::string& simpleTypeName() {
    static const std::string name(simpleTypeNameView<T>());
    return name;
}

inline jxx::Ptr<ClassAny> ensureObjectRegistered() {
    static const auto descriptor = [] {
        try {
            return ClassAny::forType(std::type_index(typeid(Object)));
        } catch (const IllegalStateException&) {
            ClassAny::Meta metadata;
            metadata.binaryName = "jxx.lang.Object";
            metadata.typeId = std::type_index(typeid(Object));
            metadata.superClass = nullptr;
            metadata.interfaces.clear();
            metadata.isInterface = false;
            metadata.isPrimitive = false;
            metadata.isArray = false;
            metadata.isEnum = false;
            metadata.isAnnotation = false;
            metadata.isSynthetic = false;
            metadata.componentType = nullptr;
            metadata.modifiers = 0x0001;
            metadata.factory = []() -> jxx::Ptr<Object> {
                return jxx::NEW<Object>();
            };
            metadata.instancePredicate =
                [](const jxx::Ptr<Object>& object) -> jbool {
                    return object != nullptr;
                };
            return ClassAny::registerClass(metadata);
        }
    }();

    return descriptor;
}

template <typename Super>
jxx::Ptr<ClassAny> superclassDescriptor() {
    if constexpr (std::is_same_v<Super, Object>) {
        return ensureObjectRegistered();
    } else if constexpr (HasClassInfo<Super>::value) {
        using ExactClassInfo =
            typename Super::JxxClassInfoMarker;

        return ExactClassInfo::Class();
    } else {
        return ClassAny::forType(std::type_index(typeid(Super)));
    }
}

template <typename Interface>
jxx::Ptr<ClassAny> interfaceDescriptor() {
    static_assert(
        HasClassInfo<Interface>::value,
        "Each Java interface listed in ClassInfo must inherit ClassInfo<Interface>."
    );

    using ExactClassInfo =
        typename Interface::JxxClassInfoMarker;

    return ExactClassInfo::Class();
}

} // namespace class_info_detail

/**
 * Metadata-only mixin for JXX Java classes and interfaces.
 *
 * Java class:
 *
 *   class Device
 *       : public Object
 *       , public ClassInfo<Device, Object, Startable, Stoppable>
 *       , public virtual Startable
 *       , public virtual Stoppable {
 *   };
 *
 * Java interface extending multiple interfaces:
 *
 *   class ManagedDevice
 *       : public ClassInfo<ManagedDevice, Object, Startable, Stoppable>
 *       , public virtual Startable
 *       , public virtual Stoppable {
 *   };
 *
 * JavaSuper is ignored when Derived does not inherit Object. It remains in the
 * template position so one ClassInfo template supports classes and interfaces.
 */
template <
    typename Derived,
    typename JavaSuper = Object,
    typename... JavaInterfaces>
class ClassInfo {
public:
    using JxxClassInfoMarker =
        ClassInfo<
            Derived,
            JavaSuper,
            JavaInterfaces...>;
    using DerivedType = Derived;
    using SuperType = JavaSuper;

    static jxx::Ptr<ClassAny> Class() {
        return ensureRegistered();
    }

    /**
     * Registration callback used by jxx::lang::initialize().
     *
     * The callback has a void return type so it can be stored as an ordinary
     * function pointer without constructing a ClassAny until initialization.
     */
    static void initializeClass() {
        (void)ensureRegistered();
    }

    static const std::string& staticClassName() {
        return class_info_detail::binaryTypeName<Derived>();
    }

    static const std::string& staticSimpleClassName() {
        return class_info_detail::simpleTypeName<Derived>();
    }

    static jxx::Ptr<ClassAny> ensureRegistered() {
        constexpr bool isJavaClass =
            std::is_base_of_v<Object, Derived>;

        static_assert(
            isJavaClass || std::is_polymorphic_v<Derived>,
            "A JXX interface must be polymorphic."
        );

        if constexpr (isJavaClass) {
            static_assert(
                std::is_base_of_v<Object, JavaSuper>,
                "The declared Java superclass must derive from Object."
            );

            static_assert(
                std::is_base_of_v<JavaSuper, Derived>,
                "Derived must inherit its declared Java superclass."
            );
        }

        static_assert(
            (std::is_base_of_v<JavaInterfaces, Derived> && ...),
            "Derived must inherit every interface listed in ClassInfo."
        );

        static const auto descriptor = [] {
            constexpr bool isClass =
                std::is_base_of_v<Object, Derived>;

            ClassAny::Meta metadata;
            metadata.binaryName = staticClassName();
            metadata.typeId = std::type_index(typeid(Derived));
            metadata.interfaces = {
                class_info_detail::interfaceDescriptor<JavaInterfaces>()...
            };
            metadata.isInterface = !isClass;
            metadata.isPrimitive = false;
            metadata.isArray = false;
            metadata.isEnum = false;
            metadata.isAnnotation = false;
            metadata.isSynthetic = false;
            metadata.componentType = nullptr;

            if constexpr (isClass) {
                metadata.superClass =
                    class_info_detail::superclassDescriptor<JavaSuper>();
                metadata.modifiers = 0x0001;

                if constexpr (std::is_abstract_v<Derived>) {
                    metadata.modifiers |= 0x0400;
                }

                if constexpr (
                    std::is_default_constructible_v<Derived> &&
                    !std::is_abstract_v<Derived>) {

                    metadata.factory = []() -> jxx::Ptr<Object> {
                        return jxx::CAST<Object>(jxx::NEW<Derived>());
                    };
                } else {
                    metadata.factory = {};
                }

                metadata.instancePredicate =
                    [](const jxx::Ptr<Object>& object) -> jbool {
                        return object != nullptr &&
                            jxx::CAST<Derived>(object) != nullptr;
                    };
            } else {
                metadata.superClass = nullptr;
                metadata.modifiers = 0x0001 | 0x0200 | 0x0400;
                metadata.factory = {};
                metadata.instancePredicate =
                    [](const jxx::Ptr<Object>& object) -> jbool {
                        return object != nullptr &&
                            std::dynamic_pointer_cast<Derived>(object) != nullptr;
                    };
            }

            return ClassAny::registerClass(metadata);
        }();

        return descriptor;
    }

protected:
    ClassInfo() = default;
    ClassInfo(const ClassInfo&) = default;
    ClassInfo(ClassInfo&&) noexcept = default;

    ClassInfo& operator=(const ClassInfo&) = default;
    ClassInfo& operator=(ClassInfo&&) noexcept = default;

    ~ClassInfo() = default;
};

/**
 * ClassInfo is the single metadata mixin for both Java classes and Java
 * interfaces. A non-Object Derived is automatically registered as an
 * interface, while an Object-derived Derived is registered as a class.
 */

/**
 * Unified base for a JXX Java interface.
 *
 * This wrapper does not inherit Object. It supplies the exact ClassInfo marker,
 * exposes Interface::Class(), and virtually inherits all parent interfaces.
 */
template <typename Derived, typename... ParentInterfaces>
class InterfaceBase
    : public ClassInfo<
          Derived,
          Object,
          ParentInterfaces...>
    , public virtual ParentInterfaces... {
private:
    using Metadata =
        ClassInfo<
            Derived,
            Object,
            ParentInterfaces...>;

public:
    using JxxClassInfoMarker = Metadata;

    /**
     * Hides Class() functions inherited through parent interfaces.
     */
    static jxx::Ptr<ClassAny> Class() {
        return Metadata::Class();
    }

    virtual ~InterfaceBase() = default;

protected:
    InterfaceBase() = default;
    InterfaceBase(const InterfaceBase&) = default;
    InterfaceBase(InterfaceBase&&) noexcept = default;

    InterfaceBase& operator=(const InterfaceBase&) = default;
    InterfaceBase& operator=(InterfaceBase&&) noexcept = default;
};

/**
 * Unified base for a JXX Java class.
 *
 * This wrapper inherits the declared Java superclass, supplies the exact
 * ClassInfo marker, exposes Derived::Class(), and virtually inherits all
 * directly implemented Java interfaces.
 */
template <
    typename Derived,
    typename JavaSuper = Object,
    typename... JavaInterfaces>
class ClassBase
    : public JavaSuper
    , public ClassInfo<
          Derived,
          JavaSuper,
          JavaInterfaces...>
    , public virtual JavaInterfaces... {
private:
    using Metadata =
        ClassInfo<
            Derived,
            JavaSuper,
            JavaInterfaces...>;

public:
    using JxxClassInfoMarker = Metadata;

    /**
     * Hides Class() functions inherited from the Java superclass and
     * implemented interfaces.
     */
    static jxx::Ptr<ClassAny> Class() {
        return Metadata::Class();
    }

    virtual ~ClassBase() = default;

protected:
    ClassBase() = default;
    ClassBase(const ClassBase&) = default;
    ClassBase(ClassBase&&) noexcept = default;

    ClassBase& operator=(const ClassBase&) = default;
    ClassBase& operator=(ClassBase&&) noexcept = default;

    template <
        typename... SuperArguments,
        typename = std::enable_if_t<
            (sizeof...(SuperArguments) > 0) &&
            std::is_constructible_v<
                JavaSuper,
                SuperArguments...>>>
    explicit ClassBase(
        SuperArguments&&... arguments)
        : JavaSuper(
              std::forward<SuperArguments>(arguments)...) {
    }
};

namespace class_info_detail {

template <typename T>
void registerClassInfo() {
    static_assert(
        HasClassInfo<T>::value,
        "registerClassInfo<T>() requires T to inherit ClassInfo.");

    using ExactClassInfo =
        typename T::JxxClassInfoMarker;

    ExactClassInfo::Class();
}

} // namespace class_info_detail

} // namespace jxx::lang
