#pragma once

#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx_types.h"

#ifdef code
#undef code
#endif
#ifdef UNSPECIFIED_EVENT_TYPE_ERR
#undef UNSPECIFIED_EVENT_TYPE_ERR
#endif

namespace jxx::org::w3c::dom::events {

class EventException : public ::jxx::lang::RuntimeException {
public:
    using JxxSuper = ::jxx::lang::RuntimeException;

    static constexpr ::jxx::lang::jshort UNSPECIFIED_EVENT_TYPE_ERR = 0;

    EventException(
        ::jxx::lang::jshort code,
        const ::jxx::Ptr<::jxx::lang::String>& message);

    EventException(const EventException& other);
    EventException(EventException&& other) noexcept;
    EventException& operator=(const EventException& other);
    EventException& operator=(EventException&& other) noexcept;
    ~EventException() override;

    ::jxx::lang::jshort code;

protected:
    const char* typeName() const noexcept override;
};

} // namespace jxx::org::w3c::dom::events
