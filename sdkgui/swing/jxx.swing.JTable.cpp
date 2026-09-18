#include "swing/jxx.swing.JTable.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
namespace jxx::swing
{
	JTable::JTable() :JTable(::jxx::NEW<DefaultTableModel>())
	{
	} JTable::JTable(const ::jxx::Ptr<TableModel>& m) :Super()
	{
		setModel(m);
	} JTable::JTable(::jxx::lang::jint r, ::jxx::lang::jint c) :JTable(::jxx::NEW<DefaultTableModel>(r, c))
	{
	}
	void JTable::setModel(const ::jxx::Ptr<TableModel>& m)
	{
		if (!m)throw ::jxx::lang::NullPointerException("model"); model_ = m; clearSelection();
	} ::jxx::Ptr<TableModel> JTable::getModel()const
	{
		return model_;
	}
	::jxx::lang::jint JTable::getRowCount()const
	{
		return model_->getRowCount();
	} ::jxx::lang::jint JTable::getColumnCount()const
	{
		return model_->getColumnCount();
	}
	::jxx::Ptr<::jxx::lang::Object> JTable::getValueAt(::jxx::lang::jint r, ::jxx::lang::jint c)const
	{
		return model_->getValueAt(r, c);
	} void JTable::setValueAt(const ::jxx::Ptr<::jxx::lang::Object>& v, ::jxx::lang::jint r, ::jxx::lang::jint c)
	{
		model_->setValueAt(v, r, c);
	}
	void JTable::setRowSelectionInterval(::jxx::lang::jint s, ::jxx::lang::jint e)
	{
		if (s < 0 || e < 0 || s >= getRowCount() || e >= getRowCount())throw ::jxx::lang::IndexOutOfBoundsException("row"); selectedRow_ = s;
	} void JTable::clearSelection()
	{
		selectedRow_ = -1;
	} ::jxx::lang::jint JTable::getSelectedRow()const
	{
		return selectedRow_;
	}
	void JTable::setRowHeight(::jxx::lang::jint v)
	{
		if (v <= 0)throw ::jxx::lang::IllegalArgumentException("height"); rowHeight_ = v;
	} ::jxx::lang::jint JTable::getRowHeight()const
	{
		return rowHeight_;
	}
	void JTable::setAutoResizeMode(::jxx::lang::jint v)
	{
		if (v < 0 || v>4)throw ::jxx::lang::IllegalArgumentException("mode"); autoResizeMode_ = v;
	} ::jxx::lang::jint JTable::getAutoResizeMode()const
	{
		return autoResizeMode_;
	}
	::jxx::Ptr<::jxx::awt::Dimension> JTable::getPreferredScrollableViewportSize()const
	{
		return ::jxx::NEW<::jxx::awt::Dimension>(getColumnCount() * 75, getRowCount() * rowHeight_);
	}
}
