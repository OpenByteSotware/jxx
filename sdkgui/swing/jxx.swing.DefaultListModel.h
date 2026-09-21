#pragma once
#include <algorithm>
#include <vector>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "swing/jxx.swing.AbstractListModel.h"
namespace jxx::swing
{
    template<typename E>
    class DefaultListModel : public ::jxx::lang::ClassBase<DefaultListModel<E>, AbstractListModel<E>>
    {
    public:
        using JxxSuper = AbstractListModel<E>;
        using Super = ::jxx::lang::ClassBase<DefaultListModel<E>, JxxSuper>;
        ::jxx::lang::jint getSize() const override { return static_cast<::jxx::lang::jint>(elements_.size()); }
        ::jxx::Ptr<E> getElementAt(::jxx::lang::jint index) const override { check(index); return elements_[static_cast<std::size_t>(index)]; }
        ::jxx::Ptr<E> get(::jxx::lang::jint index) const { return getElementAt(index); }
        void addElement(const ::jxx::Ptr<E>& value) { const auto i=getSize(); elements_.push_back(value); added(i,i); }
        void add(::jxx::lang::jint index, const ::jxx::Ptr<E>& value) { insertElementAt(value,index); }
        void insertElementAt(const ::jxx::Ptr<E>& value, ::jxx::lang::jint index) { checkInsert(index); elements_.insert(elements_.begin()+index,value); added(index,index); }
        ::jxx::Ptr<E> set(::jxx::lang::jint index, const ::jxx::Ptr<E>& value) { check(index); auto old=elements_[static_cast<std::size_t>(index)]; elements_[static_cast<std::size_t>(index)]=value; changed(index,index); return old; }
        void setElementAt(const ::jxx::Ptr<E>& value, ::jxx::lang::jint index) { (void)set(index,value); }
        ::jxx::Ptr<E> remove(::jxx::lang::jint index) { check(index); auto old=elements_[static_cast<std::size_t>(index)]; elements_.erase(elements_.begin()+index); removed(index,index); return old; }
        ::jxx::lang::jbool removeElement(const ::jxx::Ptr<E>& value) { auto i=std::find(elements_.begin(),elements_.end(),value); if(i==elements_.end()) return false; const auto index=static_cast<::jxx::lang::jint>(std::distance(elements_.begin(),i)); elements_.erase(i); removed(index,index); return true; }
        void removeElementAt(::jxx::lang::jint index) { (void)remove(index); }
        void removeRange(::jxx::lang::jint fromIndex, ::jxx::lang::jint toIndex) { if(fromIndex>toIndex) throw ::jxx::lang::IndexOutOfBoundsException("fromIndex"); check(fromIndex); check(toIndex); elements_.erase(elements_.begin()+fromIndex,elements_.begin()+toIndex+1); removed(fromIndex,toIndex); }
        void clear() { removeAllElements(); }
        void removeAllElements() { if(elements_.empty()) return; const auto last=getSize()-1; elements_.clear(); removed(0,last); }
        ::jxx::lang::jbool isEmpty() const { return elements_.empty(); }
        ::jxx::lang::jint size() const { return getSize(); }
        ::jxx::lang::jint indexOf(const ::jxx::Ptr<E>& value) const { auto i=std::find(elements_.begin(),elements_.end(),value); return i==elements_.end()?-1:static_cast<::jxx::lang::jint>(std::distance(elements_.begin(),i)); }
        ::jxx::lang::jbool contains(const ::jxx::Ptr<E>& value) const { return indexOf(value)>=0; }
        ::jxx::Ptr<E> firstElement() const { check(0); return elements_.front(); }
        ::jxx::Ptr<E> lastElement() const { check(getSize()-1); return elements_.back(); }
    private:
        ::jxx::Ptr<::jxx::lang::Object> source() { return ::jxx::CAST<::jxx::lang::Object>(this->thisPtr()); }
        void added(::jxx::lang::jint a,::jxx::lang::jint b){this->fireIntervalAdded(source(),a,b);}
        void removed(::jxx::lang::jint a,::jxx::lang::jint b){this->fireIntervalRemoved(source(),a,b);}
        void changed(::jxx::lang::jint a,::jxx::lang::jint b){this->fireContentsChanged(source(),a,b);}
        void check(::jxx::lang::jint index) const { if(index<0||index>=getSize()) throw ::jxx::lang::IndexOutOfBoundsException("index"); }
        void checkInsert(::jxx::lang::jint index) const { if(index<0||index>getSize()) throw ::jxx::lang::IndexOutOfBoundsException("index"); }
        std::vector<::jxx::Ptr<E>> elements_;
    };
}
