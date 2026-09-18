#pragma once
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "swing/jxx.swing.DefaultComboBoxModel.h"
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    template<typename E>
    class JComboBox : public ::jxx::lang::ClassBase<JComboBox<E>,JComponent>
    {
    public:
        using JxxSuper=JComponent; using Super=::jxx::lang::ClassBase<JComboBox<E>,JxxSuper>;
        JComboBox():model_(::jxx::NEW<DefaultComboBoxModel<E>>()){} explicit JComboBox(const ::jxx::Ptr<ComboBoxModel<E>>& m){setModel(m);}
        void setModel(const ::jxx::Ptr<ComboBoxModel<E>>& m){if(!m)throw ::jxx::lang::NullPointerException("model");model_=m;} ::jxx::Ptr<ComboBoxModel<E>> getModel()const{return model_;}
        void addItem(const ::jxx::Ptr<E>& item){auto m=::jxx::CAST<MutableComboBoxModel<E>>(model_);if(!m)throw ::jxx::lang::IllegalArgumentException("immutable model");m->addElement(item);}
        void removeItemAt(::jxx::lang::jint i){auto m=::jxx::CAST<MutableComboBoxModel<E>>(model_);if(!m)throw ::jxx::lang::IllegalArgumentException("immutable model");m->removeElementAt(i);}
        ::jxx::lang::jint getItemCount()const{return model_->getSize();} ::jxx::Ptr<E> getItemAt(::jxx::lang::jint i)const{return model_->getElementAt(i);}
        void setSelectedItem(const ::jxx::Ptr<E>& item){model_->setSelectedItem(item);} ::jxx::Ptr<E> getSelectedItem()const{return model_->getSelectedItem();}
        void setSelectedIndex(::jxx::lang::jint i){if(i==-1)model_->setSelectedItem(nullptr);else model_->setSelectedItem(model_->getElementAt(i));}
        ::jxx::lang::jint getSelectedIndex()const{auto value=model_->getSelectedItem();for(::jxx::lang::jint i=0;i<model_->getSize();++i)if(model_->getElementAt(i)==value)return i;return -1;}
        void setEditable(::jxx::lang::jbool v){editable_=v;} ::jxx::lang::jbool isEditable()const{return editable_;}
        void setMaximumRowCount(::jxx::lang::jint v){if(v<0)throw ::jxx::lang::IllegalArgumentException("count");maximumRows_=v;} ::jxx::lang::jint getMaximumRowCount()const{return maximumRows_;}
    private:::jxx::Ptr<ComboBoxModel<E>> model_;::jxx::lang::jbool editable_=false;::jxx::lang::jint maximumRows_=8;
    };
}
