#include "swing/jxx.swing.JSeparator.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    JSeparator::JSeparator() : Super() {}
    JSeparator::JSeparator(::jxx::lang::jint orientation) : Super() { setOrientation(orientation); }
    ::jxx::lang::jint JSeparator::getOrientation() const { return orientation_; }
    void JSeparator::setOrientation(::jxx::lang::jint value) { if (value != HORIZONTAL && value != VERTICAL) throw ::jxx::lang::IllegalArgumentException("orientation"); orientation_ = value; invalidate(); }
}
