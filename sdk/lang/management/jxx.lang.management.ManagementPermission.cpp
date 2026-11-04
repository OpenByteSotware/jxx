#include "lang/management/jxx.lang.management.ManagementPermission.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang::management {
namespace {
void validateName(const ::jxx::Ptr<::jxx::lang::String>& name) {
    if (name == nullptr) throw ::jxx::lang::NullPointerException("name");
    const auto value = name->utf8();
    if (value != "control" && value != "monitor") {
        throw ::jxx::lang::IllegalArgumentException(
            "name must be control or monitor");
    }
}
void validateActions(const ::jxx::Ptr<::jxx::lang::String>& actions) {
    if (actions != nullptr && !actions->utf8().empty()) {
        throw ::jxx::lang::IllegalArgumentException(
            "actions must be null or empty");
    }
}
} // namespace
ManagementPermission::ManagementPermission(
    const ::jxx::Ptr<::jxx::lang::String>& name)
    : Super(name) {
    validateName(name);
}
ManagementPermission::ManagementPermission(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& actions)
    : Super(name, actions) {
    validateName(name);
    validateActions(actions);
}
} // namespace jxx::lang::management
