#pragma once
#include "util/jxx.util.EventObject.h"
namespace jxx::swing::event {
class TableModelEvent : public ::jxx::lang::ClassBase<TableModelEvent, ::jxx::util::EventObject> {
public:
    using JxxSuper = ::jxx::util::EventObject;
    using Super = ::jxx::lang::ClassBase<TableModelEvent, JxxSuper>;
    static constexpr ::jxx::lang::jint INSERT = 1;
    static constexpr ::jxx::lang::jint UPDATE = 0;
    static constexpr ::jxx::lang::jint DELETE = -1;
    static constexpr ::jxx::lang::jint HEADER_ROW = -1;
    static constexpr ::jxx::lang::jint ALL_COLUMNS = -1;
    explicit TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source);
    TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint row);
    TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint firstRow, ::jxx::lang::jint lastRow);
    TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint firstRow, ::jxx::lang::jint lastRow,
        ::jxx::lang::jint column, ::jxx::lang::jint type);
    ::jxx::lang::jint getFirstRow() const;
    ::jxx::lang::jint getLastRow() const;
    ::jxx::lang::jint getColumn() const;
    ::jxx::lang::jint getType() const;
private:
    ::jxx::lang::jint firstRow_;
    ::jxx::lang::jint lastRow_;
    ::jxx::lang::jint column_;
    ::jxx::lang::jint type_;
};
}
