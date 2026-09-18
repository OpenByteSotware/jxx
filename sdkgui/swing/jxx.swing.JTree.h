#pragma once
#include "awt/jxx.awt.Dimension.h"
#include "swing/jxx.swing.DefaultTreeModel.h"
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
	class JTree : public ::jxx::lang::ClassBase<JTree, JComponent>
	{
	public:using JxxSuper = JComponent; using Super = ::jxx::lang::ClassBase<JTree, JxxSuper>; 
	JTree(); explicit JTree(const ::jxx::Ptr<TreeModel>& model);
	void setModel(const ::jxx::Ptr<TreeModel>& model);
	::jxx::Ptr<TreeModel> getModel()const; 
	void setRootVisible(::jxx::lang::jbool visible);
	::jxx::lang::jbool isRootVisible()const; 
	void setShowsRootHandles(::jxx::lang::jbool value); 
	::jxx::lang::jbool getShowsRootHandles()const; 
	void setVisibleRowCount(::jxx::lang::jint count); 
	::jxx::lang::jint getVisibleRowCount()const; 
	void setSelectionRow(::jxx::lang::jint row); 
	::jxx::lang::jint getSelectionRow()const; 
	void clearSelection(); void expandRow(::jxx::lang::jint row); 
	void collapseRow(::jxx::lang::jint row); ::jxx::lang::jbool isExpanded(::jxx::lang::jint row)const; ::jxx::Ptr<::jxx::awt::Dimension> getPreferredScrollableViewportSize()const; private:
		::jxx::Ptr<TreeModel> model_; 
		::jxx::lang::jbool rootVisible_ = true, showsRootHandles_ = false; 
		::jxx::lang::jint visibleRows_ = 20, selectionRow_ = -1; 
		std::vector<::jxx::lang::jint> expanded_;
	};
}
