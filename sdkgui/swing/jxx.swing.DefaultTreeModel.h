#pragma once
#include "swing/jxx.swing.TreeModel.h"
#include "swing/jxx.swing.TreeNode.h"
namespace jxx::swing
{
	class DefaultTreeModel : public ::jxx::lang::ClassBase<DefaultTreeModel, ::jxx::lang::Object, TreeModel>
	{
	public:using JxxSuper = ::jxx::lang::Object; using Super = ::jxx::lang::ClassBase<DefaultTreeModel, JxxSuper, TreeModel>; explicit DefaultTreeModel(const ::jxx::Ptr<TreeNode>& root); ::jxx::Ptr<::jxx::lang::Object> getRoot()const override; void setRoot(const ::jxx::Ptr<TreeNode>& root); ::jxx::Ptr<::jxx::lang::Object> getChild(const ::jxx::Ptr<::jxx::lang::Object>& parent, ::jxx::lang::jint index)const override; ::jxx::lang::jint getChildCount(const ::jxx::Ptr<::jxx::lang::Object>& parent)const override; 
		  ::jxx::lang::jbool isLeaf(const ::jxx::Ptr<::jxx::lang::Object>& node)const override; 
		  private:
			  ::jxx::Ptr<TreeNode> root_;
	};
}
