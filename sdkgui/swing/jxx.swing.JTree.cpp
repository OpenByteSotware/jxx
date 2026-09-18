#include "swing/jxx.swing.JTree.h"
#include <algorithm>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "swing/jxx.swing.DefaultMutableTreeNode.h"
namespace jxx::swing
{
    JTree::JTree():JTree(::jxx::NEW<DefaultTreeModel>(::jxx::NEW<DefaultMutableTreeNode>())){} JTree::JTree(const ::jxx::Ptr<TreeModel>& m):Super(){setModel(m);} void JTree::setModel(const ::jxx::Ptr<TreeModel>& m){if(!m)throw ::jxx::lang::NullPointerException("model");model_=m;clearSelection();expanded_.clear();expanded_.push_back(0);} ::jxx::Ptr<TreeModel> JTree::getModel()const{return model_;}
    void JTree::setRootVisible(::jxx::lang::jbool v){rootVisible_=v;} ::jxx::lang::jbool JTree::isRootVisible()const{return rootVisible_;} void JTree::setShowsRootHandles(::jxx::lang::jbool v){showsRootHandles_=v;} ::jxx::lang::jbool JTree::getShowsRootHandles()const{return showsRootHandles_;}
    void JTree::setVisibleRowCount(::jxx::lang::jint v){if(v<0)throw ::jxx::lang::IllegalArgumentException("count");visibleRows_=v;} ::jxx::lang::jint JTree::getVisibleRowCount()const{return visibleRows_;} void JTree::setSelectionRow(::jxx::lang::jint r){selectionRow_=r;} ::jxx::lang::jint JTree::getSelectionRow()const{return selectionRow_;} void JTree::clearSelection(){selectionRow_=-1;}
    void JTree::expandRow(::jxx::lang::jint r){if(std::find(expanded_.begin(),expanded_.end(),r)==expanded_.end())expanded_.push_back(r);} void JTree::collapseRow(::jxx::lang::jint r){expanded_.erase(std::remove(expanded_.begin(),expanded_.end(),r),expanded_.end());} ::jxx::lang::jbool JTree::isExpanded(::jxx::lang::jint r)const{return std::find(expanded_.begin(),expanded_.end(),r)!=expanded_.end();} ::jxx::Ptr<::jxx::awt::Dimension> JTree::getPreferredScrollableViewportSize()const{return ::jxx::NEW<::jxx::awt::Dimension>(200,visibleRows_*18);}
}
