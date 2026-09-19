#include "swing/jxx.swing.SpinnerListModel.h"
namespace jxx::swing
{
    SpinnerListModel::SpinnerListModel(const std::vector<::jxx::Ptr<::jxx::lang::Object>>& values) { setList(values); }
    ::jxx::Ptr<::jxx::lang::Object> SpinnerListModel::getValue() const { return values_[index_]; }
    void SpinnerListModel::setValue(const ::jxx::Ptr<::jxx::lang::Object>& value)
    {
        for (std::size_t i = 0; i < values_.size(); ++i) if (values_[i] == value) {
            if (index_ != i) { index_ = i; fireStateChanged(); }
            return;
        }
        throw ::jxx::lang::IllegalArgumentException("value");
    }
    ::jxx::Ptr<::jxx::lang::Object> SpinnerListModel::getNextValue() const { return index_ + 1 < values_.size() ? values_[index_ + 1] : nullptr; }
    ::jxx::Ptr<::jxx::lang::Object> SpinnerListModel::getPreviousValue() const { return index_ > 0 ? values_[index_ - 1] : nullptr; }
    const std::vector<::jxx::Ptr<::jxx::lang::Object>>& SpinnerListModel::getList() const noexcept { return values_; }
    void SpinnerListModel::setList(const std::vector<::jxx::Ptr<::jxx::lang::Object>>& values)
    {
        if (values.empty()) throw ::jxx::lang::IllegalArgumentException("empty values");
        values_ = values; index_ = 0; fireStateChanged();
    }
}
