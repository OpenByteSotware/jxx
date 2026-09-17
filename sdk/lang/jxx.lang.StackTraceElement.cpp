#include "lang/jxx.lang.StackTraceElement.h"

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

#include <cstddef>
#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {

::jxx::Ptr<ClassAny> StackTraceElement::Class() {
    return JxxClassInfoMarker::Class();
}

StackTraceElement::StackTraceElement(
    const ::jxx::Ptr<String>& declaringClass,
    const ::jxx::Ptr<String>& methodName,
    const ::jxx::Ptr<String>& fileName,
    jint lineNumber)
    : Super()
    , declaringClass_(declaringClass)
    , methodName_(methodName)
    , fileName_(fileName)
    , lineNumber_(lineNumber) {
    if (declaringClass_ == nullptr || methodName_ == nullptr) {
        throw NullPointerException();
    }
}

StackTraceElement::StackTraceElement(const StackTraceElement& other)
    : Super()
    , declaringClass_(other.declaringClass_)
    , methodName_(other.methodName_)
    , fileName_(other.fileName_)
    , lineNumber_(other.lineNumber_) {}

StackTraceElement::StackTraceElement(StackTraceElement&& other) noexcept
    : Super()
    , declaringClass_(std::move(other.declaringClass_))
    , methodName_(std::move(other.methodName_))
    , fileName_(std::move(other.fileName_))
    , lineNumber_(other.lineNumber_) {}

StackTraceElement& StackTraceElement::operator=(const StackTraceElement& other) {
    if (this != &other) {
        JxxSuper::operator=(other);
        declaringClass_ = other.declaringClass_;
        methodName_ = other.methodName_;
        fileName_ = other.fileName_;
        lineNumber_ = other.lineNumber_;
    }
    return *this;
}

StackTraceElement& StackTraceElement::operator=(StackTraceElement&& other) noexcept {
    if (this != &other) {
        JxxSuper::operator=(std::move(other));
        declaringClass_ = std::move(other.declaringClass_);
        methodName_ = std::move(other.methodName_);
        fileName_ = std::move(other.fileName_);
        lineNumber_ = other.lineNumber_;
    }
    return *this;
}

StackTraceElement::~StackTraceElement() = default;
::jxx::Ptr<String> StackTraceElement::getClassName() const { return declaringClass_; }
::jxx::Ptr<String> StackTraceElement::getMethodName() const { return methodName_; }
::jxx::Ptr<String> StackTraceElement::getFileName() const { return fileName_; }
jint StackTraceElement::getLineNumber() const { return lineNumber_; }
jbool StackTraceElement::isNativeMethod() const { return lineNumber_ == -2; }

jbool StackTraceElement::equals(const ::jxx::Ptr<Object>& other) const {
    const auto value = ::jxx::CAST<StackTraceElement>(other);
    if (value == nullptr) return false;
    const bool filesEqual = fileName_ == value->fileName_ ||
        (fileName_ != nullptr && value->fileName_ != nullptr && fileName_->equals(value->fileName_));
    return declaringClass_->equals(value->declaringClass_) &&
        methodName_->equals(value->methodName_) && filesEqual &&
        lineNumber_ == value->lineNumber_;
}

jint StackTraceElement::hashCode() const {
    jint result = declaringClass_->hashCode();
    result = 31 * result + methodName_->hashCode();
    result = 31 * result + (fileName_ == nullptr ? 0 : fileName_->hashCode());
    return 31 * result + lineNumber_;
}

::jxx::Ptr<String> StackTraceElement::toString() const {
    std::string value = declaringClass_->utf8() + "." + methodName_->utf8() + "(";
    if (isNativeMethod()) value += "Native Method";
    else if (fileName_ != nullptr && lineNumber_ >= 0) value += fileName_->utf8() + ":" + std::to_string(lineNumber_);
    else if (fileName_ != nullptr) value += fileName_->utf8();
    else value += "Unknown Source";
    value += ")";
    return ::jxx::NEW<String>(value);
}


void StackTraceElement::writeObject(
    const jxx::Ptr<jxx::io::ObjectOutputStream>& out) {
    (void)out;
}

void StackTraceElement::readObject(
    const jxx::Ptr<jxx::io::ObjectInputStream>& in) {
    (void)in;
}

void StackTraceElement::readObjectNoData() {
}

::jxx::Ptr<Object> StackTraceElement::cloneImpl() const {
    return ::jxx::CAST<Object>(::jxx::NEW<StackTraceElement>(*this));
}

} // namespace jxx::lang
