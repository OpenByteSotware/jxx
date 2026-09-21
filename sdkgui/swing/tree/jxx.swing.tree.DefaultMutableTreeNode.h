#pragma once
#include <vector>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.Object.h"
#include "swing/tree/jxx.swing.tree.TreeNode.h"
namespace jxx::swing::tree
{
	class DefaultMutableTreeNode : public ::jxx::lang::ClassBase<DefaultMutableTreeNode, ::jxx::lang::Object, TreeNode>
	{
	public:
		using JxxSuper = ::jxx::lang::Object; using Super = ::jxx::lang::ClassBase<DefaultMutableTreeNode, JxxSuper, TreeNode>;
		DefaultMutableTreeNode(); explicit DefaultMutableTreeNode(const ::jxx::Ptr<::jxx::lang::Object>& userObject); DefaultMutableTreeNode(const ::jxx::Ptr<::jxx::lang::Object>& userObject, ::jxx::lang::jbool allowsChildren);
		void add(const ::jxx::Ptr<DefaultMutableTreeNode>& child); void insert(const ::jxx::Ptr<DefaultMutableTreeNode>& child, ::jxx::lang::jint index); void remove(::jxx::lang::jint index); void removeAllChildren();
		::jxx::Ptr<TreeNode> getChildAt(::jxx::lang::jint index) const override; ::jxx::lang::jint getChildCount() const override; ::jxx::Ptr<TreeNode> getParent() const override;
		::jxx::lang::jbool getAllowsChildren() const override; void setAllowsChildren(::jxx::lang::jbool allows); ::jxx::lang::jbool isLeaf() const override;
		void setUserObject(const ::jxx::Ptr<::jxx::lang::Object>& value); ::jxx::Ptr<::jxx::lang::Object> getUserObject() const;
		private:
			::jxx::Ptr<::jxx::lang::Object> userObject_; std::weak_ptr<DefaultMutableTreeNode> parent_; std::vector<::jxx::Ptr<DefaultMutableTreeNode>> children_; ::jxx::lang::jbool allowsChildren_ = true;
	};
}
