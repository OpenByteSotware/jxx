#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class ClassAny;
class String;

template <typename Derived, typename JxxSuper, typename... JxxInterfaces>
class ClassInfo;

class StackTraceElement final : public Object {
public:
    using JxxSuper = Object;
    using JxxClassInfoMarker = ClassInfo<StackTraceElement, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    StackTraceElement(
        const ::jxx::Ptr<String>& declaringClass,
        const ::jxx::Ptr<String>& methodName,
        const ::jxx::Ptr<String>& fileName,
        jint lineNumber);

    StackTraceElement(const StackTraceElement& other);
    StackTraceElement(StackTraceElement&& other) noexcept;
    StackTraceElement& operator=(const StackTraceElement& other);
    StackTraceElement& operator=(StackTraceElement&& other) noexcept;
    ~StackTraceElement() override;

    ::jxx::Ptr<String> getClassName() const;
    ::jxx::Ptr<String> getMethodName() const;
    ::jxx::Ptr<String> getFileName() const;
    jint getLineNumber() const;
    jbool isNativeMethod() const;

    jbool equals(const ::jxx::Ptr<Object>& other) const override;
    jint hashCode() const override;
    ::jxx::Ptr<String> toString() const override;

protected:
    ::jxx::Ptr<Object> cloneImpl() const override;

private:
    ::jxx::Ptr<String> declaringClass_;
    ::jxx::Ptr<String> methodName_;
    ::jxx::Ptr<String> fileName_;
    jint lineNumber_;
};

} // namespace jxx::lang
