#pragma once
#include <algorithm>
#include <vector>
#include "awt/jxx.awt.Dimension.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/jxx.swing.DefaultListModel.h"
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    template<typename E>
    class JList : public ::jxx::lang::ClassBase<JList<E>, JComponent>
    {
    public:
        using JxxSuper=JComponent; using Super=::jxx::lang::ClassBase<JList<E>,JxxSuper>;
        static constexpr ::jxx::lang::jint VERTICAL=0,VERTICAL_WRAP=1,HORIZONTAL_WRAP=2;
        JList() : model_(::jxx::NEW<DefaultListModel<E>>()) {}
        explicit JList(const ::jxx::Ptr<ListModel<E>>& model) { setModel(model); }
        void setModel(const ::jxx::Ptr<ListModel<E>>& model) { if(!model) throw ::jxx::lang::NullPointerException("model"); model_=model; clearSelection(); }
        ::jxx::Ptr<ListModel<E>> getModel() const { return model_; }
        void setSelectedIndex(::jxx::lang::jint index) { if(index < -1 || index >= model_->getSize()) throw ::jxx::lang::IndexOutOfBoundsException("index"); selected_.clear(); if(index >= 0) selected_.push_back(index); }
        ::jxx::lang::jint getSelectedIndex() const { return selected_.empty() ? -1 : selected_.front(); }
        ::jxx::Ptr<E> getSelectedValue() const { auto i=getSelectedIndex(); return i < 0 ? nullptr : model_->getElementAt(i); }
        void addSelectionInterval(::jxx::lang::jint start,::jxx::lang::jint end) { if(start>end) std::swap(start,end); for(auto i=start;i<=end;++i) if(i>=0&&i<model_->getSize()&&std::find(selected_.begin(),selected_.end(),i)==selected_.end()) selected_.push_back(i); std::sort(selected_.begin(),selected_.end()); }
        void clearSelection() { selected_.clear(); }
        ::jxx::lang::jbool isSelectionEmpty() const { return selected_.empty(); }
        void setVisibleRowCount(::jxx::lang::jint count) { visibleRows_=count; } ::jxx::lang::jint getVisibleRowCount() const { return visibleRows_; }
        void setLayoutOrientation(::jxx::lang::jint value) { if(value<0||value>2) throw ::jxx::lang::IllegalArgumentException("layoutOrientation"); orientation_=value; }
        ::jxx::lang::jint getLayoutOrientation() const { return orientation_; }
        ::jxx::Ptr<::jxx::awt::Dimension> getPreferredScrollableViewportSize() const { return ::jxx::NEW<::jxx::awt::Dimension>(160,std::max(1,visibleRows_)*20); }
    private:
        ::jxx::Ptr<ListModel<E>> model_; std::vector<::jxx::lang::jint> selected_; ::jxx::lang::jint visibleRows_=8,orientation_=VERTICAL;
    };
}
