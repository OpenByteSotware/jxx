#include "swing/jxx.swing.DefaultTableModel.h"
#include <algorithm>
#include <string>
namespace jxx::swing
{
    DefaultTableModel::DefaultTableModel() = default;
    DefaultTableModel::DefaultTableModel(::jxx::lang::jint rows, ::jxx::lang::jint columns) { setColumnCount(columns); setRowCount(rows); }
    ::jxx::lang::jint DefaultTableModel::getRowCount() const { return static_cast<::jxx::lang::jint>(data_.size()); }
    ::jxx::lang::jint DefaultTableModel::getColumnCount() const { return static_cast<::jxx::lang::jint>(columns_.size()); }
    ::jxx::Ptr<::jxx::lang::String> DefaultTableModel::getColumnName(::jxx::lang::jint column) const { if(column<0||column>=getColumnCount()) throw ::jxx::lang::IndexOutOfBoundsException("column"); return columns_[column] ? columns_[column] : ::jxx::NEW<::jxx::lang::String>(std::string(1, static_cast<char>('A'+column%26))); }
    void DefaultTableModel::check(::jxx::lang::jint row,::jxx::lang::jint column) const { if(row<0||row>=getRowCount()||column<0||column>=getColumnCount()) throw ::jxx::lang::IndexOutOfBoundsException("cell"); }
    ::jxx::Ptr<::jxx::lang::Object> DefaultTableModel::getValueAt(::jxx::lang::jint row,::jxx::lang::jint column) const { check(row,column); return data_[row][column]; }
    void DefaultTableModel::setValueAt(const ::jxx::Ptr<::jxx::lang::Object>& value,::jxx::lang::jint row,::jxx::lang::jint column) { check(row,column); data_[row][column]=value; }
    ::jxx::lang::jbool DefaultTableModel::isCellEditable(::jxx::lang::jint,::jxx::lang::jint) const { return true; }
    void DefaultTableModel::setRowCount(::jxx::lang::jint rows) { if(rows<0) throw ::jxx::lang::IndexOutOfBoundsException("rows"); data_.resize(rows); for(auto& row:data_) row.resize(columns_.size()); }
    void DefaultTableModel::setColumnCount(::jxx::lang::jint columns) { if(columns<0) throw ::jxx::lang::IndexOutOfBoundsException("columns"); columns_.resize(columns); for(auto& row:data_) row.resize(columns); }
    void DefaultTableModel::addRow(const std::vector<::jxx::Ptr<::jxx::lang::Object>>& row) { auto copy=row; copy.resize(columns_.size()); data_.push_back(std::move(copy)); }
    void DefaultTableModel::removeRow(::jxx::lang::jint row) { if(row<0||row>=getRowCount()) throw ::jxx::lang::IndexOutOfBoundsException("row"); data_.erase(data_.begin()+row); }
    void DefaultTableModel::setColumnIdentifiers(const std::vector<::jxx::Ptr<::jxx::lang::String>>& identifiers) { columns_=identifiers; for(auto& row:data_) row.resize(columns_.size()); }
}
