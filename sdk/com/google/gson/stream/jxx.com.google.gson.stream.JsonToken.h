#pragma once
namespace com::google::gson::stream {
enum class JsonToken {
    BEGIN_ARRAY,
    END_ARRAY,
    BEGIN_OBJECT,
    END_OBJECT,
    NAME,
    STRING,
    NUMBER,
    BOOLEAN,
    NULL_VALUE,
    END_DOCUMENT
};
} // namespace com::google::gson::stream
