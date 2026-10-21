#include "com/jxx.com.ClassInitialization.h"
#include "com/google/jxx.com.google.ClassInitialization.h"
#include "com/sun/jxx.com.sun.ClassInitialization.h"

namespace com
{
void initialize()
{
    ::com::google::initialize();
    ::com::sun::initialize();
}

} // namespace com
