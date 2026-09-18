#pragma once
#include <algorithm>
#include <vector>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/jxx.swing.ListModel.h"
namespace jxx::swing
{
    template<typename E>
    class DefaultListModel : public ::jxx::lang::ClassBase<DefaultListModel<E>, ::jxx::lang::Object, ListModel<E>>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<DefaultListModel<E>, JxxSuper, ListModel<E>>;
        ::jxx::lang::jint getSize() const override { return static_cast<::jxx::lang::jint>(elements_.size()); }
        ::jxx::Ptr<E> getElementAt(::jxx::lang::jint index) const override { check(index); return elements_[static_cast<std::size_t>(index)]; }
        void addElement(const ::jxx::Ptr<E>& value) { elements_.push_back(value); }
        void insertElementAt(const ::jxx::Ptr<E>& value, ::jxx::lang::jint index) { if(index < 0 || index > getSize()) throw ::jxx::lang::IndexOutOfBoundsException("index"); elements_.insert(elements_.begin()+index,value); }
        void setElementAt(const ::jxx::Ptr<E>& value, ::jxx::lang::jint index) { check(index); elements_[static_cast<std::size_t>(index)] = value; }
        ::jxx::lang::jbool removeElement(const ::jxx::Ptr<E>& value) { auto i=std::find(elements_.begin(),elements_.end(),value); if(i==elements_.end()) return false; elements_.erase(i); return true; }
        void removeElementAt(::jxx::lang::jint index) { check(index); elements_.erase(elements_.begin()+index); }
        void removeAllElements() { elements_.clear(); }
    private:
        void check(::jxx::lang::jint index) const { if(index < 0 || index >= getSize()) throw ::jxx::lang::IndexOutOfBoundsException("index"); }
        std::vector<::jxx::Ptr<E>> elements_;
    };
}
