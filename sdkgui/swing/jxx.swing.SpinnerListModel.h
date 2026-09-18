#pragma once
#include <vector>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.SpinnerModel.h"
namespace jxx::swing
{
    class SpinnerListModel : public ::jxx::lang::ClassBase<SpinnerListModel,::jxx::lang::Object,SpinnerModel>
    {
    public:
        using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<SpinnerListModel,JxxSuper,SpinnerModel>;
        explicit SpinnerListModel(const std::vector<::jxx::Ptr<::jxx::lang::Object>>& values):values_(values){if(values_.empty())throw ::jxx::lang::IllegalArgumentException("empty values");}
        ::jxx::Ptr<::jxx::lang::Object> getValue()const override{return values_[index_];}
        void setValue(const ::jxx::Ptr<::jxx::lang::Object>& v)override{for(std::size_t i=0;i<values_.size();++i)if(values_[i]==v){index_=i;return;}throw ::jxx::lang::IllegalArgumentException("value");}
        ::jxx::Ptr<::jxx::lang::Object> getNextValue()const override{return index_+1<values_.size()?values_[index_+1]:nullptr;}
        ::jxx::Ptr<::jxx::lang::Object> getPreviousValue()const override{return index_>0?values_[index_-1]:nullptr;}
    private:std::vector<::jxx::Ptr<::jxx::lang::Object>> values_;std::size_t index_=0;
    };
}
