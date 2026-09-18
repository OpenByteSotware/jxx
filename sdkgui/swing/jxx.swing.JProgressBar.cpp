#include "swing/jxx.swing.JProgressBar.h"
#include <string>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing
{
	JProgressBar::JProgressBar() :JProgressBar(HORIZONTAL)
	{
	}JProgressBar::JProgressBar(::jxx::lang::jint o) :Super(), model_(::jxx::NEW<DefaultBoundedRangeModel>())
	{
		setOrientation(o);
	}JProgressBar::JProgressBar(::jxx::lang::jint n, ::jxx::lang::jint x) :Super(), model_(::jxx::NEW<DefaultBoundedRangeModel>(n, 0, n, x))
	{
	}void JProgressBar::setModel(const ::jxx::Ptr<BoundedRangeModel>& m)
	{
		if (!m)throw ::jxx::lang::NullPointerException("model"); model_ = m;
	}::jxx::Ptr<BoundedRangeModel> JProgressBar::getModel()const
	{
		return model_;
	}::jxx::lang::jint JProgressBar::getValue()const
	{
		return model_->getValue();
	}
	void JProgressBar::setValue(::jxx::lang::jint v)
	{
		model_->setValue(v);
	}::jxx::lang::jint JProgressBar::getMinimum()const
	{
		return model_->getMinimum();
	}void JProgressBar::setMinimum(::jxx::lang::jint v)
	{
		model_->setMinimum(v);
	}::jxx::lang::jint JProgressBar::getMaximum()const
	{
		return model_->getMaximum();
	}void JProgressBar::setMaximum(::jxx::lang::jint v)
	{
		model_->setMaximum(v);
	}void JProgressBar::setOrientation(::jxx::lang::jint v)
	{
		if (v != HORIZONTAL && v != VERTICAL)throw ::jxx::lang::IllegalArgumentException("orientation"); orientation_ = v;
	}::jxx::lang::jint JProgressBar::getOrientation()const
	{
		return orientation_;
	}void JProgressBar::setStringPainted(::jxx::lang::jbool v)
	{
		stringPainted_ = v;
	}::jxx::lang::jbool JProgressBar::isStringPainted()const
	{
		return stringPainted_;
	}void JProgressBar::setString(const ::jxx::Ptr<::jxx::lang::String>& v)
	{
		string_ = v;
	}::jxx::Ptr<::jxx::lang::String> JProgressBar::getString()const
	{
		return string_ ? string_ : ::jxx::NEW<::jxx::lang::String>(std::to_string(static_cast<int>(getPercentComplete() * 100.0)) + "%");
	}void JProgressBar::setIndeterminate(::jxx::lang::jbool v)
	{
		indeterminate_ = v;
	}::jxx::lang::jbool JProgressBar::isIndeterminate()const
	{
		return indeterminate_;
	}::jxx::lang::jdouble JProgressBar::getPercentComplete()const
	{
		const auto range = getMaximum() - getMinimum(); return range <= 0 ? 0.0 : static_cast<::jxx::lang::jdouble>(getValue() - getMinimum()) / range;
	}
}
