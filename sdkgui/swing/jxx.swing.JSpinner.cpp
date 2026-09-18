#include "swing/jxx.swing.JSpinner.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing
{
    JSpinner::JSpinner(const ::jxx::Ptr<SpinnerModel>& m):Super(){setModel(m);}
    void JSpinner::setModel(const ::jxx::Ptr<SpinnerModel>& m){if(!m)throw ::jxx::lang::NullPointerException("model");model_=m;}
    ::jxx::Ptr<SpinnerModel> JSpinner::getModel()const{return model_;}
    ::jxx::Ptr<::jxx::lang::Object> JSpinner::getValue()const{return model_->getValue();}
    void JSpinner::setValue(const ::jxx::Ptr<::jxx::lang::Object>& v){model_->setValue(v);}
    ::jxx::Ptr<::jxx::lang::Object> JSpinner::getNextValue()const{return model_->getNextValue();}
    ::jxx::Ptr<::jxx::lang::Object> JSpinner::getPreviousValue()const{return model_->getPreviousValue();}
}
