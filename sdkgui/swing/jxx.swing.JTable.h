#pragma once
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "swing/jxx.swing.DefaultTableModel.h"
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
	class JTable : public ::jxx::lang::ClassBase<JTable, JComponent>
	{
	public:
		using JxxSuper = JComponent; using Super = ::jxx::lang::ClassBase<JTable, JxxSuper>;
		static constexpr ::jxx::lang::jint AUTO_RESIZE_OFF = 0, AUTO_RESIZE_NEXT_COLUMN = 1, AUTO_RESIZE_SUBSEQUENT_COLUMNS = 2, AUTO_RESIZE_LAST_COLUMN = 3, AUTO_RESIZE_ALL_COLUMNS = 4;
		JTable(); explicit JTable(const ::jxx::Ptr<TableModel>& model); JTable(::jxx::lang::jint rows, ::jxx::lang::jint columns);
		void setModel(const ::jxx::Ptr<TableModel>& model); ::jxx::Ptr<TableModel> getModel() const;
		::jxx::lang::jint getRowCount() const; ::jxx::lang::jint getColumnCount() const;
		::jxx::Ptr<::jxx::lang::Object> getValueAt(::jxx::lang::jint row, ::jxx::lang::jint column) const;
		void setValueAt(const ::jxx::Ptr<::jxx::lang::Object>& value, ::jxx::lang::jint row, ::jxx::lang::jint column);
		void setRowSelectionInterval(::jxx::lang::jint start, ::jxx::lang::jint end); void clearSelection(); ::jxx::lang::jint getSelectedRow() const;
		void setRowHeight(::jxx::lang::jint height); ::jxx::lang::jint getRowHeight() const;
		void setAutoResizeMode(::jxx::lang::jint mode); ::jxx::lang::jint getAutoResizeMode() const;
		::jxx::Ptr<::jxx::awt::Dimension> getPreferredScrollableViewportSize() const;
		private:
			::jxx::Ptr<TableModel> model_; ::jxx::lang::jint selectedRow_ = -1, rowHeight_ = 16, autoResizeMode_ = AUTO_RESIZE_SUBSEQUENT_COLUMNS;
	};
}
