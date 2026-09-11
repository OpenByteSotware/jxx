#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class Object;
class String;
}

namespace jxx::org::w3c::dom {

class Node;

class UserDataHandler
    : public ::jxx::lang::InterfaceBase<UserDataHandler> {
public:
    static constexpr ::jxx::lang::jshort NODE_CLONED = 1;
    static constexpr ::jxx::lang::jshort NODE_IMPORTED = 2;
    static constexpr ::jxx::lang::jshort NODE_DELETED = 3;
    static constexpr ::jxx::lang::jshort NODE_RENAMED = 4;
    static constexpr ::jxx::lang::jshort NODE_ADOPTED = 5;

    ~UserDataHandler() override = default;

    virtual void handle(
        ::jxx::lang::jshort operation,
        const ::jxx::Ptr<::jxx::lang::String>& key,
        const ::jxx::Ptr<::jxx::lang::Object>& data,
        const ::jxx::Ptr<Node>& source,
        const ::jxx::Ptr<Node>& destination) = 0;
};

} // namespace jxx::org::w3c::dom
