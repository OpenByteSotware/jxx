#pragma once
#include <algorithm>
#include <vector>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/jxx.swing.MutableComboBoxModel.h"
namespace jxx::swing
{
    template<typename E>
    class DefaultComboBoxModel : public ::jxx::lang::ClassBase<DefaultComboBoxModel<E>,::jxx::lang::Object,MutableComboBoxModel<E>>
    {
    public:
        using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<DefaultComboBoxModel<E>,JxxSuper,MutableComboBoxModel<E>>;
        ::jxx::lang::jint getSize() const override{return static_cast<::jxx::lang::jint>(items_.size());}
        ::jxx::Ptr<E> getElementAt(::jxx::lang::jint i)const override{check(i);return items_[i];}
        void setSelectedItem(const ::jxx::Ptr<E>& item)override{selected_=item;} ::jxx::Ptr<E> getSelectedItem()const override{return selected_;}
        void addElement(const ::jxx::Ptr<E>& item)override{items_.push_back(item);if(items_.size()==1)selected_=item;}
        void insertElementAt(const ::jxx::Ptr<E>& item,::jxx::lang::jint i)override{if(i<0||i>getSize())throw ::jxx::lang::IndexOutOfBoundsException("index");items_.insert(items_.begin()+i,item);if(items_.size()==1)selected_=item;}
        void removeElement(const ::jxx::Ptr<E>& item)override{auto i=std::find(items_.begin(),items_.end(),item);if(i!=items_.end())removeElementAt(static_cast<::jxx::lang::jint>(i-items_.begin()));}
        void removeElementAt(::jxx::lang::jint i)override{check(i);auto old=items_[i];items_.erase(items_.begin()+i);if(selected_==old)selected_=items_.empty()?nullptr:items_[std::min<std::size_t>(i,items_.size()-1)];}
    private:void check(::jxx::lang::jint i)const{if(i<0||i>=getSize())throw ::jxx::lang::IndexOutOfBoundsException("index");}std::vector<::jxx::Ptr<E>> items_;::jxx::Ptr<E> selected_;
    };
}
