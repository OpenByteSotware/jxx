#include "awt/jxx.awt.GridBagConstraints.h"
namespace jxx::awt {
GridBagConstraints::GridBagConstraints():insets(::jxx::NEW<Insets>(0,0,0,0)){}
GridBagConstraints::GridBagConstraints(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint w,::jxx::lang::jint h,::jxx::lang::jdouble wx,::jxx::lang::jdouble wy,::jxx::lang::jint a,::jxx::lang::jint f,const ::jxx::Ptr<Insets>& i,::jxx::lang::jint px,::jxx::lang::jint py):gridx(x),gridy(y),gridwidth(w),gridheight(h),weightx(wx),weighty(wy),anchor(a),fill(f),ipadx(px),ipady(py),insets(i?::jxx::NEW<Insets>(i->top,i->left,i->bottom,i->right) : ::jxx::NEW<Insets>(0,0,0,0)){}
::jxx::Ptr<::jxx::lang::Object> GridBagConstraints::clone() const{return ::jxx::NEW<GridBagConstraints>(gridx,gridy,gridwidth,gridheight,weightx,weighty,anchor,fill,insets,ipadx,ipady);}
}
