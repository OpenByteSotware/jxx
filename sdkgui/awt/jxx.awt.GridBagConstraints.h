#pragma once
#include "awt/jxx.awt.Insets.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt {
class GridBagConstraints : public ::jxx::lang::ClassBase<GridBagConstraints,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<GridBagConstraints,JxxSuper>;
 static constexpr ::jxx::lang::jint RELATIVE=-1,REMAINDER=0;
 static constexpr ::jxx::lang::jint NONE=0,BOTH=1,HORIZONTAL=2,VERTICAL=3;
 static constexpr ::jxx::lang::jint CENTER=10,NORTH=11,NORTHEAST=12,EAST=13,SOUTHEAST=14,SOUTH=15,SOUTHWEST=16,WEST=17,NORTHWEST=18;
 static constexpr ::jxx::lang::jint PAGE_START=19,PAGE_END=20,LINE_START=21,LINE_END=22,FIRST_LINE_START=23,FIRST_LINE_END=24,LAST_LINE_START=25,LAST_LINE_END=26;
 static constexpr ::jxx::lang::jint BASELINE=256,BASELINE_LEADING=512,BASELINE_TRAILING=768,ABOVE_BASELINE=1024,ABOVE_BASELINE_LEADING=1280,ABOVE_BASELINE_TRAILING=1536,BELOW_BASELINE=1792,BELOW_BASELINE_LEADING=2048,BELOW_BASELINE_TRAILING=2304;
 ::jxx::lang::jint gridx=RELATIVE,gridy=RELATIVE,gridwidth=1,gridheight=1;
 ::jxx::lang::jdouble weightx=0.0,weighty=0.0;
 ::jxx::lang::jint anchor=CENTER,fill=NONE,ipadx=0,ipady=0;
 ::jxx::Ptr<Insets> insets;
 GridBagConstraints();
 GridBagConstraints(::jxx::lang::jint gridx,::jxx::lang::jint gridy,::jxx::lang::jint gridwidth,::jxx::lang::jint gridheight,::jxx::lang::jdouble weightx,::jxx::lang::jdouble weighty,::jxx::lang::jint anchor,::jxx::lang::jint fill,const ::jxx::Ptr<Insets>& insets,::jxx::lang::jint ipadx,::jxx::lang::jint ipady);
 ::jxx::Ptr<::jxx::lang::Object> clone() const override;
}; }
