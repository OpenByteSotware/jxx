#include "swing/event/jxx.swing.event.TableModelEvent.h"
namespace jxx::swing::event {
TableModelEvent::TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source) : TableModelEvent(source, 0, 2147483647, ALL_COLUMNS, UPDATE) {}
TableModelEvent::TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint row) : TableModelEvent(source, row, row, ALL_COLUMNS, UPDATE) {}
TableModelEvent::TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint firstRow, ::jxx::lang::jint lastRow) : TableModelEvent(source, firstRow, lastRow, ALL_COLUMNS, UPDATE) {}
TableModelEvent::TableModelEvent(const ::jxx::Ptr<::jxx::lang::Object>& source, ::jxx::lang::jint firstRow, ::jxx::lang::jint lastRow, ::jxx::lang::jint column, ::jxx::lang::jint type)
    : Super(source), firstRow_(firstRow), lastRow_(lastRow), column_(column), type_(type) {}
::jxx::lang::jint TableModelEvent::getFirstRow() const { return firstRow_; }
::jxx::lang::jint TableModelEvent::getLastRow() const { return lastRow_; }
::jxx::lang::jint TableModelEvent::getColumn() const { return column_; }
::jxx::lang::jint TableModelEvent::getType() const { return type_; }
}
