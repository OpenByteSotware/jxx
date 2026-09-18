#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::swing
{
    class TreeNode : public ::jxx::lang::InterfaceBase<TreeNode>
    {
    public:
        ~TreeNode() override=default;
        virtual ::jxx::Ptr<TreeNode> getChildAt(::jxx::lang::jint index) const=0;
        virtual ::jxx::lang::jint getChildCount() const=0;
        virtual ::jxx::Ptr<TreeNode> getParent() const=0;
        virtual ::jxx::lang::jbool getAllowsChildren() const=0;
        virtual ::jxx::lang::jbool isLeaf() const=0;
    };
}
