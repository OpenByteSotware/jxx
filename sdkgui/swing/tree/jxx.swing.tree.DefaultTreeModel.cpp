#include "swing/tree/jxx.swing.tree.DefaultTreeModel.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing::tree
{
    DefaultTreeModel::DefaultTreeModel(const ::jxx::Ptr<TreeNode>& r){setRoot(r);} ::jxx::Ptr<::jxx::lang::Object> DefaultTreeModel::getRoot()const{return ::jxx::CAST<::jxx::lang::Object>(root_);} void DefaultTreeModel::setRoot(const ::jxx::Ptr<TreeNode>& r){if(!r)throw ::jxx::lang::NullPointerException("root");root_=r;} ::jxx::Ptr<::jxx::lang::Object> DefaultTreeModel::getChild(const ::jxx::Ptr<::jxx::lang::Object>& p,::jxx::lang::jint i)const{return ::jxx::CAST<::jxx::lang::Object>(::jxx::CAST<TreeNode>(p)->getChildAt(i));} ::jxx::lang::jint DefaultTreeModel::getChildCount(const ::jxx::Ptr<::jxx::lang::Object>& p)const{return ::jxx::CAST<TreeNode>(p)->getChildCount();} ::jxx::lang::jbool DefaultTreeModel::isLeaf(const ::jxx::Ptr<::jxx::lang::Object>& n)const{return ::jxx::CAST<TreeNode>(n)->isLeaf();}
}
