#include "awt/jxx.awt.Cursor.h"

#include <sstream>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    std::array<::jxx::Ptr<Cursor>, 14> Cursor::predefined_{};

    const char* Cursor::predefinedName(::jxx::lang::jint type)
    {
        static constexpr const char* names[] = {
            "Default Cursor", "Crosshair Cursor", "Text Cursor",
            "Wait Cursor", "Southwest Resize Cursor",
            "Southeast Resize Cursor", "Northwest Resize Cursor",
            "Northeast Resize Cursor", "North Resize Cursor",
            "South Resize Cursor", "West Resize Cursor",
            "East Resize Cursor", "Hand Cursor", "Move Cursor"
        };
        return names[type];
    }

    Cursor::Cursor(::jxx::lang::jint type)
        : name(nullptr), type_(type)
    {
        if (type < DEFAULT_CURSOR || type > MOVE_CURSOR)
            throw ::jxx::lang::IllegalArgumentException("type");
        name = ::jxx::NEW<::jxx::lang::String>(predefinedName(type));
    }

    Cursor::Cursor(const ::jxx::Ptr<::jxx::lang::String>& value)
        : name(value), type_(CUSTOM_CURSOR)
    {
        if (name == nullptr)
            throw ::jxx::lang::IllegalArgumentException("name");
    }

    ::jxx::Ptr<Cursor> Cursor::createCustom(
        const ::jxx::Ptr<::jxx::lang::String>& value)
    {
        return ::jxx::Ptr<Cursor>(new Cursor(value));
    }

    ::jxx::Ptr<Cursor> Cursor::getDefaultCursor()
    {
        return getPredefinedCursor(DEFAULT_CURSOR);
    }

    ::jxx::Ptr<Cursor> Cursor::getPredefinedCursor(::jxx::lang::jint type)
    {
        if (type < DEFAULT_CURSOR || type > MOVE_CURSOR)
            throw ::jxx::lang::IllegalArgumentException("type");
        auto& cursor = predefined_[static_cast<std::size_t>(type)];
        if (cursor == nullptr) cursor = ::jxx::NEW<Cursor>(type);
        return cursor;
    }

    ::jxx::Ptr<Cursor> Cursor::getSystemCustomCursor(
        const ::jxx::Ptr<::jxx::lang::String>& value)
    {
        if (value == nullptr)
            throw ::jxx::lang::IllegalArgumentException("name");
        return createCustom(value);
    }

    ::jxx::lang::jint Cursor::getType() const { return type_; }
    ::jxx::Ptr<::jxx::lang::String> Cursor::getName() const { return name; }

    ::jxx::Ptr<::jxx::lang::String> Cursor::toString() const
    {
        std::ostringstream out;
        out << "jxx.awt.Cursor[" << name->utf8() << "]";
        return ::jxx::NEW<::jxx::lang::String>(out.str());
    }
}
