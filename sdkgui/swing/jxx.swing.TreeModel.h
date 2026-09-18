#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::swing
{
    class TreeModel : public ::jxx::lang::InterfaceBase<TreeModel>
    {
    public:~TreeModel() override=default;virtual ::jxx::Ptr<::jxx::lang::Object> getRoot()const=0;virtual ::jxx::Ptr<::jxx::lang::Object> getChild(const ::jxx::Ptr<::jxx::lang::Object>& parent,::jxx::lang::jint index)const=0;virtual ::jxx::lang::jint getChildCount(const ::jxx::Ptr<::jxx::lang::Object>& parent)const=0;virtual ::jxx::lang::jbool isLeaf(const ::jxx::Ptr<::jxx::lang::Object>& node)const=0;
    };
}
