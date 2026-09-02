#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

/*
 * ClassInfo, ClassBase, and InterfaceBase are C++17 templates.
 * Their definitions must remain in jxx.lang.ClassInfo.h.
 *
 * Ordinary interface descriptors are resolved generically through each
 * interface's JxxClassInfoMarker. This translation unit intentionally does
 * not include Serializable, Cloneable, CharSequence, Comparable, or other
 * interface headers, which prevents reflection include cycles.
 */

} // namespace jxx::lang
