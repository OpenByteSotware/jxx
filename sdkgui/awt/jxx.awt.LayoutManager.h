#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
namespace jxx::awt { class Component; class Container; class Dimension;
class LayoutManager : public ::jxx::lang::InterfaceBase<LayoutManager> {
public:
    ~LayoutManager() override=default;
    virtual void addLayoutComponent(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<Component>& component)=0;
    virtual void removeLayoutComponent(const ::jxx::Ptr<Component>& component)=0;
    virtual ::jxx::Ptr<Dimension> preferredLayoutSize(const ::jxx::Ptr<Container>& parent)=0;
    virtual ::jxx::Ptr<Dimension> minimumLayoutSize(const ::jxx::Ptr<Container>& parent)=0;
    virtual void layoutContainer(const ::jxx::Ptr<Container>& parent)=0;
}; }
