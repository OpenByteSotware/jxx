#pragma once
#include "swing/jxx.swing.DefaultBoundedRangeModel.h"
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
namespace jxx::swing
{
	class JSlider : public ::jxx::lang::ClassBase<JSlider, JComponent, SwingConstants>
	{
	public:using JxxSuper = JComponent; using Super = ::jxx::lang::ClassBase<JSlider, JxxSuper, SwingConstants>; JSlider(); explicit JSlider(::jxx::lang::jint orientation); JSlider(::jxx::lang::jint minimum, ::jxx::lang::jint maximum); JSlider(::jxx::lang::jint minimum, ::jxx::lang::jint maximum, ::jxx::lang::jint value); ::jxx::lang::jint getValue()const; void setValue(::jxx::lang::jint value); ::jxx::lang::jint getMinimum()const; void setMinimum(::jxx::lang::jint value); ::jxx::lang::jint getMaximum()const; void setMaximum(::jxx::lang::jint value); void setOrientation(::jxx::lang::jint orientation); ::jxx::lang::jint getOrientation()const; void setMajorTickSpacing(::jxx::lang::jint value); ::jxx::lang::jint getMajorTickSpacing()const; void setMinorTickSpacing(::jxx::lang::jint value); ::jxx::lang::jint getMinorTickSpacing()const; void setPaintTicks(::jxx::lang::jbool value); ::jxx::lang::jbool getPaintTicks()const; void setSnapToTicks(::jxx::lang::jbool value); 
		  ::jxx::lang::jbool getSnapToTicks()const; void setModel(const ::jxx::Ptr<BoundedRangeModel>& model); ::jxx::Ptr<BoundedRangeModel> getModel()const; ::jxx::lang::jbool getValueIsAdjusting()const; void setValueIsAdjusting(::jxx::lang::jbool value); void setInverted(::jxx::lang::jbool value); ::jxx::lang::jbool getInverted()const; void setPaintTrack(::jxx::lang::jbool value); ::jxx::lang::jbool getPaintTrack()const; void setPaintLabels(::jxx::lang::jbool value); ::jxx::lang::jbool getPaintLabels()const;
		  private:
			  ::jxx::Ptr<BoundedRangeModel> model_; ::jxx::lang::jint orientation_ = HORIZONTAL, major_ = 0, minor_ = 0; ::jxx::lang::jbool paintTicks_ = false, snap_ = false, inverted_ = false, paintTrack_ = true, paintLabels_ = false;
	};
}
