#pragma once
#include <vector>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "swing/jxx.swing.AbstractSpinnerModel.h"
namespace jxx::swing
{
    class SpinnerListModel : public ::jxx::lang::ClassBase<SpinnerListModel, AbstractSpinnerModel>
    {
    public:
        using JxxSuper = AbstractSpinnerModel;
        using Super = ::jxx::lang::ClassBase<SpinnerListModel, JxxSuper>;
        explicit SpinnerListModel(const std::vector<::jxx::Ptr<::jxx::lang::Object>>& values);
        ::jxx::Ptr<::jxx::lang::Object> getValue() const override;
        void setValue(const ::jxx::Ptr<::jxx::lang::Object>& value) override;
        ::jxx::Ptr<::jxx::lang::Object> getNextValue() const override;
        ::jxx::Ptr<::jxx::lang::Object> getPreviousValue() const override;
        const std::vector<::jxx::Ptr<::jxx::lang::Object>>& getList() const noexcept;
        void setList(const std::vector<::jxx::Ptr<::jxx::lang::Object>>& values);
    private:
        std::vector<::jxx::Ptr<::jxx::lang::Object>> values_;
        std::size_t index_ = 0;
    };
}
