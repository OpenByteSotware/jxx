#include <type_traits>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Appendable.h"
#include "lang/jxx.lang.AutoCloseable.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Iterable.h"
#include "lang/jxx.lang.Readable.h"
#include "lang/jxx.lang.Runnable.h"

#define JXX_INTERFACE(T) static_assert(!std::is_base_of_v<::jxx::lang::Object, T>, #T " must be an interface and must not inherit Object")
#define JXX_EXTENDS(T, B) static_assert(std::is_base_of_v<B, T>, #T " is missing documented parent interface " #B)

JXX_INTERFACE(::jxx::lang::Appendable);
JXX_INTERFACE(::jxx::lang::AutoCloseable);
JXX_INTERFACE(::jxx::lang::CharSequence);
JXX_INTERFACE(::jxx::lang::Cloneable);
JXX_INTERFACE(::jxx::lang::Comparable<::jxx::lang::Object>);
JXX_INTERFACE(::jxx::lang::Iterable<::jxx::lang::Object>);
JXX_INTERFACE(::jxx::lang::Readable);
JXX_INTERFACE(::jxx::lang::Runnable);

#undef JXX_EXTENDS
#undef JXX_INTERFACE
