#include <type_traits>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Appendable.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Compiler.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.InheritableThreadLocal.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Math.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Process.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Runtime.h"
#include "lang/jxx.lang.SecurityManager.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.StackTraceElement.h"
#include "lang/jxx.lang.StrictMath.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuffer.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.ThreadGroup.h"
#include "lang/jxx.lang.ThreadLocal.h"
#include "lang/jxx.lang.Void.h"

#define JXX_CLASS(T) static_assert(std::is_base_of_v<::jxx::lang::Object, T>, #T " must derive from Object")
#define JXX_BASE(T, B) static_assert(std::is_base_of_v<B, T>, #T " is missing documented base/interface " #B)
#define JXX_NOT_BASE(T, B) static_assert(!std::is_base_of_v<B, T>, #T " incorrectly derives from " #B)

JXX_CLASS(::jxx::lang::Boolean); JXX_BASE(::jxx::lang::Boolean, ::jxx::lang::Comparable<::jxx::lang::Boolean>); JXX_BASE(::jxx::lang::Boolean, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Byte); JXX_BASE(::jxx::lang::Byte, ::jxx::lang::Number); JXX_BASE(::jxx::lang::Byte, ::jxx::lang::Comparable<::jxx::lang::Byte>); JXX_BASE(::jxx::lang::Byte, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Character); JXX_BASE(::jxx::lang::Character, ::jxx::lang::Comparable<::jxx::lang::Character>); JXX_BASE(::jxx::lang::Character, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Double); JXX_BASE(::jxx::lang::Double, ::jxx::lang::Number); JXX_BASE(::jxx::lang::Double, ::jxx::lang::Comparable<::jxx::lang::Double>); JXX_BASE(::jxx::lang::Double, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Float); JXX_BASE(::jxx::lang::Float, ::jxx::lang::Number); JXX_BASE(::jxx::lang::Float, ::jxx::lang::Comparable<::jxx::lang::Float>); JXX_BASE(::jxx::lang::Float, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Integer); JXX_BASE(::jxx::lang::Integer, ::jxx::lang::Number); JXX_BASE(::jxx::lang::Integer, ::jxx::lang::Comparable<::jxx::lang::Integer>); JXX_BASE(::jxx::lang::Integer, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Long); JXX_BASE(::jxx::lang::Long, ::jxx::lang::Number); JXX_BASE(::jxx::lang::Long, ::jxx::lang::Comparable<::jxx::lang::Long>); JXX_BASE(::jxx::lang::Long, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Short); JXX_BASE(::jxx::lang::Short, ::jxx::lang::Number); JXX_BASE(::jxx::lang::Short, ::jxx::lang::Comparable<::jxx::lang::Short>); JXX_BASE(::jxx::lang::Short, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Number); JXX_BASE(::jxx::lang::Number, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::String); JXX_BASE(::jxx::lang::String, ::jxx::lang::CharSequence); JXX_BASE(::jxx::lang::String, ::jxx::lang::Comparable<::jxx::lang::String>); JXX_BASE(::jxx::lang::String, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::StringBuffer); JXX_BASE(::jxx::lang::StringBuffer, ::jxx::lang::Appendable); JXX_BASE(::jxx::lang::StringBuffer, ::jxx::lang::CharSequence); JXX_BASE(::jxx::lang::StringBuffer, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::StringBuilder); JXX_BASE(::jxx::lang::StringBuilder, ::jxx::lang::Appendable); JXX_BASE(::jxx::lang::StringBuilder, ::jxx::lang::CharSequence); JXX_BASE(::jxx::lang::StringBuilder, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::StackTraceElement); JXX_BASE(::jxx::lang::StackTraceElement, ::jxx::io::SerializableI);
JXX_CLASS(::jxx::lang::Thread); JXX_BASE(::jxx::lang::Thread, ::jxx::lang::Runnable);
JXX_CLASS(::jxx::lang::ThreadGroup); JXX_BASE(::jxx::lang::ThreadGroup, ::jxx::lang::Thread::UncaughtExceptionHandler);
JXX_CLASS(::jxx::lang::ClassLoader); JXX_CLASS(::jxx::lang::Compiler); JXX_CLASS(::jxx::lang::Math); JXX_CLASS(::jxx::lang::Process); JXX_CLASS(::jxx::lang::ProcessBuilder); JXX_CLASS(::jxx::lang::Runtime); JXX_CLASS(::jxx::lang::SecurityManager); JXX_CLASS(::jxx::lang::StrictMath); JXX_CLASS(::jxx::lang::System); JXX_CLASS(::jxx::lang::Void);
JXX_CLASS(::jxx::lang::ThreadLocal<::jxx::lang::Object>); JXX_CLASS(::jxx::lang::InheritableThreadLocal<::jxx::lang::Object>); JXX_BASE(::jxx::lang::InheritableThreadLocal<::jxx::lang::Object>, ::jxx::lang::ThreadLocal<::jxx::lang::Object>);

#undef JXX_NOT_BASE
#undef JXX_BASE
#undef JXX_CLASS
